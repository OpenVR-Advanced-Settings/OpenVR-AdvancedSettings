#include "AudioProfileController.h"
#include <QQuickWindow>
#include <QApplication>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings {


	//TODO DOCS/verify / other parts
  void AudioProfileController::reloadAudioProfiles() {
    audioProfiles.clear();
    auto settings = OverlayController::appSettings();
    settings->beginGroup(getSettingsName());
    auto profileCount = settings->beginReadArray("audioProfiles");
    for (int i = 0; i < profileCount; i++) {
      settings->setArrayIndex(i);
      audioProfiles.emplace_back();
      auto& entry = audioProfiles[i];
      entry.profileName = settings->value("profileName").toString().toStdString();
    }
    settings->endArray();
    settings->endGroup();
  }

  void AudioProfileController::saveAudioProfiles() {
    auto settings = OverlayController::appSettings();
    settings->beginGroup(getSettingsName());
    settings->beginWriteArray("audioProfiles");
    unsigned i = 0;
    for (auto& p : audioProfiles) {
      settings->setArrayIndex(i);
      settings->setValue("profileName", QString::fromStdString(p.profileName));
      i++;
    }
    settings->endArray();
    settings->endGroup();
  }




  void AudioProfileController::addAudioProfile(QString name) {
    AudioProfile* profile = nullptr;
    for (auto& p : audioProfiles) {
      if (p.profileName.compare(name.toStdString()) == 0) {
        profile = &p;
        break;
      }
    }
    if (!profile) {
      auto i = audioProfiles.size();
      audioProfiles.emplace_back();
      profile = &audioProfiles[i];
    }
    profile->profileName = name.toStdString();
    saveAudioProfiles();
    OverlayController::appSettings()->sync();
    emit audioProfilesUpdated();
  }

  void AudioProfileController::applyAudioProfile(unsigned index) {
	  //DO i need?
    std::lock_guard<std::recursive_mutex> lock(eventLoopMutexAudio);
    if (index < audioProfiles.size()) {
      auto& profile = audioProfiles[index];
	  //TODO apply logic here
      //setPttShowNotification(profile.showNotification);

    }
  }

  void AudioProfileController::deleteAudioProfile(unsigned index) {
    if (index < audioProfiles.size()) {
      auto pos = audioProfiles.begin() + index;
      audioProfiles.erase(pos);
      saveAudioProfiles();
      OverlayController::appSettings()->sync();
      emit audioProfilesUpdated();
    }
  }



  unsigned AudioProfileController::getAudioProfileCount() {
    return (unsigned)audioProfiles.size();
  }

  QString AudioProfileController::getAudioProfileName(unsigned index) {
    if (index < audioProfiles.size()) {
      return QString::fromStdString(audioProfiles[index].profileName);
    }
    return "";
  }




  /*void logControllerState(const vr::VRControllerState_t& state, const std::string& prefix) {
    if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu)) {
      LOG(INFO) << prefix << vr::VRSystem()->GetButtonIdNameFromEnum(vr::k_EButton_ApplicationMenu) << " pressed";
    }
  }
  */

}
//namespace advconfig