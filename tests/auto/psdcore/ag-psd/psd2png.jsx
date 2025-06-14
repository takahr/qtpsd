#target photoshop

// Photoshop -> File -> Script -> Browse...
// Choose tests/auto/psdcore/ag-psd/ag-psd/tests
// Choose docs/images/psd2png
// Photoshop may ask something e.g. link errror of D:\Downloads\output.mp3

function exportAsPng(doc, outputFile) {
    var options = new ExportOptionsSaveForWeb();
    options.format = SaveDocumentType.PNG;
    options.PNG8 = false; // true = PNG-8, false = PNG-24
    options.transparency = true;
    options.interlaced = false;
    options.quality = 100;

    doc.exportDocument(outputFile, ExportType.SAVEFORWEB, options);
}

function convertPsdToPng(file, inputRoot, outputRoot) {
    var relativePath = file.path.replace(inputRoot.fullName, "");
    var outputFolder = new Folder(outputRoot.fullName + relativePath);
    if (!outputFolder.exists) outputFolder.create();

    var doc = null;
    try {
        doc = open(file);
        var pngFile = new File(outputFolder + "/" + file.name.replace(/\.psd$/i, ".png"));
        exportAsPng(doc, pngFile);
    } catch (e) {
        $.writeln("Failed to open or export: " + file.fsName + "\nReason: " + e);
    } finally {
        if (doc) {
            try {
                doc.close(SaveOptions.DONOTSAVECHANGES);
            } catch (e) {
                $.writeln("Failed to close document: " + file.fsName + "\nReason: " + e);
            }
        }
    }
}

function processFolder(folder, inputRoot, outputRoot) {
    var files = folder.getFiles();
    for (var i = 0; i < files.length; i++) {
        var f = files[i];
        if (f instanceof Folder) {
            processFolder(f, inputRoot, outputRoot);
        } else if (f instanceof File && f.name.match(/\.psd$/i)) {
            convertPsdToPng(f, inputRoot, outputRoot);
        }
    }
}

var inputFolder = Folder.selectDialog("PSDファイルのルートフォルダを選択してください");
var outputFolder = Folder.selectDialog("PNGの出力先フォルダを選択してください");

if (inputFolder && outputFolder) {
    processFolder(inputFolder, inputFolder, outputFolder);
    alert("変換が完了しました！");
}

