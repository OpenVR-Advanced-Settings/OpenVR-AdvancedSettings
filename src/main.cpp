#include "overlaycontroller.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QSettings>
#include <QStandardPaths>
#include <openvr.h>
#include <iostream>
#include "logging.h"

const char* logConfigFileName = "logging.conf";

const char* logConfigDefault =
"* GLOBAL:\n"
"	FORMAT = \"[%level] %datetime{%Y-%M-%d %H:%m:%s}: %msg\"\n"
"	FILENAME = \"AdvancedSettings.log\"\n"
"	ENABLED = true\n"
"	TO_FILE = true\n"
"	TO_STANDARD_OUTPUT = true\n"
"	MAX_LOG_FILE_SIZE = 2097152 ## 2MB\n"
"* TRACE:\n"
"	ENABLED = false\n"
"* DEBUG:\n"
"	ENABLED = false\n";

INITIALIZE_EASYLOGGINGPP

void myQtMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
	QByteArray localMsg = msg.toLocal8Bit();
	switch (type) {
		case QtDebugMsg:
			LOG(DEBUG) << localMsg.constData() << " (" << context.file << ":" << context.line << ")";
			break;
		case QtInfoMsg:
			LOG(INFO) << localMsg.constData() << " (" << context.file << ":" << context.line << ")";
			break;
		case QtWarningMsg:
			LOG(WARNING) << localMsg.constData() << " (" << context.file << ":" << context.line << ")";
			break;
		case QtCriticalMsg:
			LOG(ERROR) << localMsg.constData() << " (" << context.file << ":" << context.line << ")";
			break;
		case QtFatalMsg:
			LOG(FATAL) << localMsg.constData() << " (" << context.file << ":" << context.line << ")";
			break;
	}
}

void installManifest(bool cleaninstall = false) {
	auto manifestQPath = QDir::cleanPath(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("manifest.vrmanifest"));
	if (QFile::exists(manifestQPath)) {
		bool alreadyInstalled = false;
		if (vr::VRApplications()->IsApplicationInstalled(advsettings::OverlayController::applicationKey)) {
			if (cleaninstall) {
				char buffer[1024];
				auto appError = vr::VRApplicationError_None;
				vr::VRApplications()->GetApplicationPropertyString(advsettings::OverlayController::applicationKey, vr::VRApplicationProperty_WorkingDirectory_String, buffer, 1024, &appError);
				if (appError == vr::VRApplicationError_None) {
					auto oldManifestQPath = QDir::cleanPath(QDir(buffer).absoluteFilePath("manifest.vrmanifest"));
					if (oldManifestQPath.compare(manifestQPath, Qt::CaseInsensitive) != 0) {
						vr::VRApplications()->RemoveApplicationManifest(QDir::toNativeSeparators(oldManifestQPath).toStdString().c_str());
					} else {
						alreadyInstalled = true;
					}
				}
			} else {
				alreadyInstalled = true;
			}
		}
		auto apperror = vr::VRApplications()->AddApplicationManifest(QDir::toNativeSeparators(manifestQPath).toStdString().c_str());
		if (apperror != vr::VRApplicationError_None) {
			throw std::runtime_error(std::string("Could not add application manifest: ") + std::string(vr::VRApplications()->GetApplicationsErrorNameFromEnum(apperror)));
		} else if (!alreadyInstalled || cleaninstall) {
			auto apperror = vr::VRApplications()->SetApplicationAutoLaunch(advsettings::OverlayController::applicationKey, true);
			if (apperror != vr::VRApplicationError_None) {
				throw std::runtime_error(std::string("Could not set auto start: ") + std::string(vr::VRApplications()->GetApplicationsErrorNameFromEnum(apperror)));
			}
		}
	} else {
		throw std::runtime_error(std::string("Could not find application manifest: ") + manifestQPath.toStdString());
	}
}

void removeManifest() {
	auto manifestQPath = QDir::cleanPath(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("manifest.vrmanifest"));
	if (QFile::exists(manifestQPath)) {
		if (vr::VRApplications()->IsApplicationInstalled(advsettings::OverlayController::applicationKey)) {
			vr::VRApplications()->RemoveApplicationManifest(QDir::toNativeSeparators(manifestQPath).toStdString().c_str());
		}
	} else {
		throw std::runtime_error(std::string("Could not find application manifest: ") + manifestQPath.toStdString());
	}
}


int main(int argc, char *argv[]) {
	
	bool desktopMode = false;
	bool noSound = false;
	bool noManifest = false;

	// Parse command line arguments
	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]).compare("-desktop") == 0) {
			desktopMode = true;
		} else if (std::string(argv[i]).compare("-nosound") == 0) {
			noSound = true;
		} else if (std::string(argv[i]).compare("-nomanifest") == 0) {
			noManifest = true;
		} else if (std::string(argv[i]).compare("-installmanifest") == 0) {
			int exitcode = 0;
			QCoreApplication coreApp(argc, argv);
			auto initError = vr::VRInitError_None;
			vr::VR_Init(&initError, vr::VRApplication_Utility);
			if (initError == vr::VRInitError_None) {
				try {
					installManifest(true);
				} catch (std::exception& e) {
					exitcode = -1;
					std::cerr << e.what() << std::endl;
				}
			} else {
				exitcode = -2;
				std::cerr << std::string("Failed to initialize OpenVR: " + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(initError))) << std::endl;
			}
			vr::VR_Shutdown();
			exit(exitcode);
		} else if (std::string(argv[i]).compare("-removemanifest") == 0) {
			int exitcode = 0;
			QCoreApplication coreApp(argc, argv);
			auto initError = vr::VRInitError_None;
			vr::VR_Init(&initError, vr::VRApplication_Utility);
			if (initError == vr::VRInitError_None) {
				try {
					removeManifest();
				} catch (std::exception& e) {
					exitcode = -1;
					std::cerr << e.what() << std::endl;
				}
			} else {
				exitcode = -2;
				std::cerr << std::string("Failed to initialize OpenVR: " + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(initError))) << std::endl;
			}
			vr::VR_Shutdown();
			exit(exitcode);
		}
	}

	try {
		QApplication a(argc, argv);
		a.setOrganizationName("matzman666");
		a.setApplicationName("OpenVRAdvancedSettings");
		a.setApplicationDisplayName(advsettings::OverlayController::applicationName);
		a.setApplicationVersion(advsettings::OverlayController::applicationVersionString);

		qInstallMessageHandler(myQtMessageHandler);

		// Configure logger
		QString logFilePath;
		START_EASYLOGGINGPP(argc, argv);
		el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
		auto logconfigfile = QFileInfo(logConfigFileName).absoluteFilePath();
		el::Configurations conf;
		if (QFile::exists(logconfigfile)) {
			conf.parseFromFile(logconfigfile.toStdString());
		} else {
			conf.parseFromText(logConfigDefault);
		}
		if (!conf.get(el::Level::Global, el::ConfigurationType::Filename)) {
			logFilePath = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).absoluteFilePath("AdvancedSettings.log");
			conf.set(el::Level::Global, el::ConfigurationType::Filename, QDir::toNativeSeparators(logFilePath).toStdString());
		}
		conf.setRemainingToDefault();
		el::Loggers::reconfigureAllLoggers(conf);
		LOG(INFO) << "Application started";
		LOG(INFO) << "Log Config: " << QDir::toNativeSeparators(logconfigfile).toStdString();
		if (!logFilePath.isEmpty()) {
			LOG(INFO) << "Log File: " << logFilePath;
		}
		
		if (desktopMode) {
			LOG(INFO) << "Desktop mode enabled.";
		}
		if (noSound) {
			LOG(INFO) << "Sound effects disabled.";
		}
		if (noManifest) {
			LOG(INFO) << "vrmanifest disabled.";
		}

		QSettings appSettings(QSettings::IniFormat, QSettings::UserScope, a.organizationName(), a.applicationName());
		advsettings::OverlayController::setAppSettings(&appSettings);
		LOG(INFO) << "Settings File: " << appSettings.fileName().toStdString();

		QQmlEngine qmlEngine;

		advsettings::OverlayController* controller = advsettings::OverlayController::createInstance(desktopMode, noSound);
		controller->Init(&qmlEngine);

		QQmlComponent component(&qmlEngine, QUrl::fromLocalFile("res/qml/mainwidget.qml"));
		auto errors = component.errors();
		for (auto& e : errors) {
			LOG(ERROR) << "QML Error: " << e.toString().toStdString() << std::endl;
		}
		auto quickObj = component.create();
		controller->SetWidget(qobject_cast<QQuickItem*>(quickObj), advsettings::OverlayController::applicationName, advsettings::OverlayController::applicationKey);

		if (!desktopMode && !noManifest) {
			try {
				installManifest();
			} catch (std::exception& e) {
				LOG(ERROR) << e.what();
			}
		}

		if (desktopMode) {
			auto m_pWindow = new QQuickWindow();
			qobject_cast<QQuickItem*>(quickObj)->setParentItem(m_pWindow->contentItem());
			m_pWindow->setGeometry(0, 0, qobject_cast<QQuickItem*>(quickObj)->width(), qobject_cast<QQuickItem*>(quickObj)->height());
			m_pWindow->show();
		}

		return a.exec();

	} catch (const std::exception& e) {
		LOG(FATAL) << e.what();
		return -1;
	}
	return 0;
}
