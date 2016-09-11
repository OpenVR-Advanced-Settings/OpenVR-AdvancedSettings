#include "overlaywidget.h"
#include "overlaycontroller.h"
#include <QApplication>
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
"	FORMAT = \"[%level] %datetime{%Y-%M-%d %H:%m:%s} %func: %msg\"\n";

INITIALIZE_EASYLOGGINGPP


int main(int argc, char *argv[]) {
	// Configure logger
	START_EASYLOGGINGPP(argc, argv);
	el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
	if (QFile::exists(logConfigFileName)) {
		el::Configurations conf(logConfigFileName);
		el::Loggers::reconfigureAllLoggers(conf);
	} else {
		el::Configurations conf;
		conf.parseFromText(logConfigDefault);
		el::Loggers::reconfigureAllLoggers(conf);
	}

	LOG(INFO) << "Starting Application.";
	try {
		QApplication a(argc, argv);
		advsettings::OverlayWidget *pOverlayWidget = new advsettings::OverlayWidget;
		advsettings::OverlayController* controller = new advsettings::OverlayController();

		controller->Init();
		controller->SetWidget(pOverlayWidget, advsettings::OverlayController::applicationName, advsettings::OverlayController::applicationKey);

		std::string manifestPath = QApplication::applicationDirPath().toStdString() + "\\advancedsettings.vrmanifest";
		if (QFile::exists(QString::fromStdString(manifestPath))) {
			bool firstTime = false;
			if (!vr::VRApplications()->IsApplicationInstalled(advsettings::OverlayController::applicationKey)) {
				firstTime;
			}
			auto apperror = vr::VRApplications()->AddApplicationManifest(manifestPath.c_str());
			if (apperror != vr::VRApplicationError_None) {
				LOG(ERROR) << "Could not add application manifest: " << vr::VRApplications()->GetApplicationsErrorNameFromEnum(apperror);
			} else if (firstTime) {
				auto apperror = vr::VRApplications()->SetApplicationAutoLaunch(advsettings::OverlayController::applicationKey, true);
				if (apperror != vr::VRApplicationError_None) {
					LOG(ERROR) << "Could not set auto start: " << vr::VRApplications()->GetApplicationsErrorNameFromEnum(apperror);
				}
			}
		} else {
			LOG(ERROR) << "Could not find application manifest: " << manifestPath;
		}

		return a.exec();

	} catch (const std::exception& e) {
		LOG(FATAL) << e.what();
		return -1;
	}
	return 0;
}
