#include "qpsdabstractplugin.h"

#include <windows.h>

QDir QPsdAbstractPlugin::qpsdPluginDir(const QString &type)
{
    HMODULE hModule = NULL;
    if (GetModuleHandleEx(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(&qpsdPluginDir),
            &hModule)) {

        WCHAR path[MAX_PATH];
        if (GetModuleFileName(hModule, path, MAX_PATH) != 0) {
            QFileInfo fi(QString::fromWCharArray(path));
            QDir pluginsDir = fi.absoluteDir();
            pluginsDir.cdUp();
            pluginsDir.cd("plugins");
            pluginsDir.cd(type);
            return pluginsDir;
        } else {
            qWarning() << "GetModuleFileNameに失敗しました。";
        }
    } else {
        qWarning() << "GetModuleHandleExに失敗しました。";
    }
    return QDir();
}
