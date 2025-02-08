#pragma once

#include <qcontainerfwd.h>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

class ProjectGenerator {
public:
    ProjectGenerator() = default;

    ~ProjectGenerator() = default;

    static bool generate(const QString &name, const QString &path, bool packetio, bool widgettools,
                         QString &msg);

private:
    static bool _copy_dir(const QDir &fromDir, const QDir &toDir, bool coverIfFileExists = true);

    static void _copy_file(const QString &source, const QString &dest);

    template <typename... Args>
    static void _template2file(const QString &source, const QString &dest, Args &&...args);
};

inline bool ProjectGenerator::_copy_dir(const QDir &fromDir, const QDir &toDir,
                                        bool coverIfFileExists) {
    if (!toDir.exists() && !toDir.mkpath(".")) {
        return false;
    }

    foreach (const QFileInfo &fileInfo,
             fromDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
        QString destPath = toDir.filePath(fileInfo.fileName());
        if (fileInfo.isDir()) {
            if (!_copy_dir(QDir(fileInfo.filePath()), QDir(destPath), coverIfFileExists)) {
                return false;
            }
        } else {
            if (coverIfFileExists && QFile::exists(destPath)) {
                QFile::remove(destPath);
            }
            if (!QFile::copy(fileInfo.filePath(), destPath)) {
                return false;
            }
        }
    }
    return true;
}

template <typename... Args>
inline void ProjectGenerator::_template2file(const QString &source, const QString &dest,
                                             Args &&...args) {
    QFile file(source);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString content = in.readAll().arg(std::forward<Args>(args)...);
        file.close();

        QDir outputDir = QFileInfo(dest).absoluteDir();
        if (!outputDir.exists()) {
            outputDir.mkpath(".");
        }

        QFile outputFile(dest);
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outputFile);
            out << content;
            outputFile.close();
        } else {
            qDebug() << "Failed to open output file.";
        }
    } else {
        qDebug() << "Failed to open resource file.";
    }
}

inline void ProjectGenerator::_copy_file(const QString &source, const QString &dest) {
    QFile::copy(source, dest);
    QFile::setPermissions(
        dest, QFile::WriteOwner | QFile::WriteUser | QFile::WriteGroup | QFile::WriteOther);
}

inline bool ProjectGenerator::generate(const QString &name, const QString &path, bool packetio,
                                       bool widgettools, QString &msg) {
    QDir rootDir(QApplication::applicationDirPath() + "../../../");

    if (!QFileInfo(rootDir.filePath("template")).exists() ||
        !QFileInfo(rootDir.filePath("Install")).exists()) {
        msg = "The template folder or Install folder does not exist";
        return false;
    }

    if (name.isEmpty()) {
        msg = "The name cannot be empty";
        return false;
    }

    if (path.isEmpty()) {
        msg = "The path cannot be empty";
        return false;
    }

    QDir projectRootDir(path);
    if (!projectRootDir.exists()) {
        msg = "The path does not exist";
        return false;
    }

    QDir projectDir(projectRootDir.filePath(name));
    if (projectDir.exists()) {
        msg = QString("%1 folder already exists").arg(name);
        return false;
    }

    if (!_copy_dir(rootDir.filePath("template"), projectDir)) {
        msg = "Failed to copy template directory";
        return false;
    }

    auto set_package_info = [](bool condition, const QString &package_name) {
        QString find_package, include_dir, library_dir, library;
        if (condition) {
            find_package = QString(
                               "\nfind_package(%1 REQUIRED PATHS "
                               "${CMAKE_SOURCE_DIR}/3rdparty/%1/lib/cmake NO_DEFAULT_PATH)\n")
                               .arg(package_name);
            include_dir = QString("${%1_INCLUDE_DIRS}\n\t\t").arg(package_name);
            library_dir = QString("${%1_LIBRARY_DIRS}\n\t\t").arg(package_name);
            library = QString("%1\n\t\t").arg(package_name);
        }
        return QStringList{find_package, include_dir, library_dir, library};
    };

    QStringList widgettools_list = set_package_info(widgettools, "ElaWidgetTools");
    QStringList packetio_list = set_package_info(packetio, "ElaPacketIO");

    _template2file(projectDir.filePath("CMakeLists.txt"), projectDir.filePath("CMakeLists.txt"),
                   name, widgettools_list[0], packetio_list[0], widgettools_list[1],
                   packetio_list[1], widgettools_list[2], packetio_list[2], widgettools_list[3],
                   packetio_list[3]);

    _template2file(projectDir.filePath("README.md"), projectDir.filePath("README.md"), name);

    if (packetio && !_copy_dir(rootDir.filePath("Install/ElaPacketIO"),
                               projectDir.filePath("3rdparty/ElaPacketIO"))) {
        msg = "Failed to copy ElaPacketIO directory";
        return false;
    }

    if (widgettools && !_copy_dir(rootDir.filePath("Install/ElaWidgetTools"),
                                  projectDir.filePath("3rdparty/ElaWidgetTools"))) {
        msg = "Failed to copy ElaWidgetTools directory";
        return false;
    }

    msg = "Project created successfully";
    return true;
}