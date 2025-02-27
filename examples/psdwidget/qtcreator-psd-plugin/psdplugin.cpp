// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "psdpluginconstants.h"
#include "psdplugintr.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditorfactory.h>
#include <coreplugin/idocument.h>

#include <extensionsystem/iplugin.h>

#include <utils/filepath.h>
#include <utils/qtcassert.h>
#include <utils/styledbar.h>
#include <utils/stylehelper.h>

#include <QtPsdCore/qpsdparser.h>
#include <QtPsdWidget/qpsdview.h>
#include <QtPsdWidget/qpsdwidgettreeitemmodel.h>

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileInfo>
#include <QToolBar>
#include <QToolButton>
#include <QCoreApplication>
#include <QSpacerItem>
#include <memory>

using namespace Core;
using namespace Utils;

namespace PsdPlugin::Internal {

// Forward declarations
class PsdViewer;
class PsdViewerFile;

// PsdViewerFile - Document class for PSD files
class PsdViewerFile : public IDocument
{
    Q_OBJECT

public:
    PsdViewerFile()
        : m_parser(new QPsdParser())
    {
        setId(Constants::PSDVIEWER_ID);
    }

    ~PsdViewerFile() override
    {
        delete m_parser;
    }

    OpenResult open(QString *errorString, const FilePath &filePath,
                    const FilePath &realFilePath) override
    {
        QTC_CHECK(filePath == realFilePath); // does not support auto save
        
        try {
            m_parser->load(filePath.toUrlishString());
            setFilePath(filePath);
            setMimeType("image/vnd.adobe.photoshop");
            emit openFinished(true);
            return OpenResult::Success;
        } catch (const std::exception &e) {
            if (errorString)
                *errorString = Tr::tr("Failed to open PSD file: %1").arg(QString::fromUtf8(e.what()));
            emit openFinished(false);
            return OpenResult::CannotHandle;
        }
    }

    Utils::Result saveImpl(const Utils::FilePath &filePath = {}, bool autoSave = false) override
    {
        Q_UNUSED(filePath)
        Q_UNUSED(autoSave)
        return Utils::Result(false, Tr::tr("Saving PSD files is not supported.")); // Read-only for now
    }

    bool setContents(const QByteArray &contents) override
    {
        Q_UNUSED(contents)
        return false; // Not implemented
    }

    bool isModified() const override
    {
        return false; // Read-only for now
    }

    bool isSaveAsAllowed() const override
    {
        return false; // Read-only for now
    }

    ReloadBehavior reloadBehavior(ChangeTrigger state, ChangeType type) const override
    {
        if (type == TypeRemoved)
            return BehaviorSilent;
        if (type == TypeContents && state == TriggerInternal && !isModified())
            return BehaviorSilent;
        return BehaviorAsk;
    }

    Result reload(ReloadFlag flag, ChangeType type) override
    {
        Q_UNUSED(type)
        if (flag == FlagIgnore)
            return Result::Ok;
            
        emit aboutToReload();
        
        try {
            m_parser->load(filePath().toUrlishString());
            emit reloadFinished(true);
            return Result::Ok;
        } catch (const std::exception &e) {
            const QString errorString = Tr::tr("Failed to reload PSD file: %1").arg(QString::fromUtf8(e.what()));
            emit reloadFinished(false);
            return Result(false, errorString);
        }
    }

    QPsdParser* parser() const
    {
        return m_parser;
    }

signals:
    void openFinished(bool success);
    void aboutToReload();
    void reloadFinished(bool success);

private:
    QPsdParser *m_parser;
};

// PsdViewer - Editor class for PSD files
class PsdViewer : public IEditor
{
    Q_OBJECT

public:
    PsdViewer(const std::shared_ptr<PsdViewerFile> &document)
        : m_file(document)
    {
        m_psdView = new QPsdView();
        m_model = new QPsdWidgetTreeItemModel(this);

        setContext(Context(Constants::PSDVIEWER_ID));
        setWidget(m_psdView);
        setDuplicateSupported(true);

        // Create toolbar
        m_toolbar = new StyledBar;

        connect(m_file.get(), &PsdViewerFile::openFinished, this, &PsdViewer::updateView);
        connect(m_file.get(), &PsdViewerFile::reloadFinished, this, &PsdViewer::updateView);

        m_psdView->setModel(m_model);
    }
    PsdViewer()
        : PsdViewer(nullptr)
    {}

    ~PsdViewer() override
    {
        delete m_psdView;
        delete m_toolbar;
    }

    IDocument *document() const override
    {
        return m_file.get();
    }

    QWidget *toolBar() override
    {
        return m_toolbar;
    }

    IEditor *duplicate() override
    {
        auto other = new PsdViewer(m_file);
        other->m_psdView->setModel(m_model);
        
        emit editorDuplicated(other);
        return other;
    }

private slots:
    void updateView()
    {
        if (m_file && m_file->parser()) {
            m_model->fromParser(*m_file->parser());
        }
    }

private:
    std::shared_ptr<PsdViewerFile> m_file;
    QPsdView *m_psdView;
    QPsdWidgetTreeItemModel *m_model;
    QWidget *m_toolbar;
};

class PsdViewerFactory final : public IEditorFactory
{
public:
    PsdViewerFactory()
    {
        setId(Constants::PSDVIEWER_ID);
        setDisplayName(Tr::tr("PSD Viewer"));
        setEditorCreator([] { return new PsdViewer; });
        
        // Register the PSD MIME type
        addMimeType(QLatin1String("image/vnd.adobe.photoshop"));
        addMimeType(QLatin1String("image/x-photoshop"));
        addMimeType(QLatin1String("image/x-psd"));
        addMimeType(QLatin1String("application/photoshop"));
        addMimeType(QLatin1String("application/x-photoshop"));
        addMimeType(QLatin1String("application/vnd.adobe.photoshop"));
    }
};

class PsdPlugin final : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "PsdPlugin.json")

public:
    void initialize() final
    {
        static PsdViewerFactory thePsdViewerFactory;
    }
};

} // namespace PsdPlugin::Internal

#include "psdplugin.moc"
