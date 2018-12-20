
#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <string>
#include <mutex>
#include <openvr.h>
#include "PttController.h"
//#include "AudioTabController.h"

class QQuickWindow;
// application namespace
namespace advsettings {

  // forward declaration
  class OverlayController;

  struct AudioProfile {
    std::string profileName;
	std::string playbackName;
	std::string mirrorName;
	std::string micName;
	float mirrorVol = 0.0;
	float micVol = 0.0;
	bool micMute = false;
	bool mirrorMute = false;
	//TODO all the settings!!! huehue
    //bool showNotification = false;
  };


  class AudioProfileController : public PttController {
    Q_OBJECT
    //Q_PROPERTY(bool pttActive READ pttActive NOTIFY pttActiveChanged)
	//NEED TO DOUBLE CHECK THIS IS CORRECT!?

  protected:

	std::vector<AudioProfile> audioProfiles;

 protected:
	virtual QString getSettingsName() = 0;
	//may have to change to prevent issues
    std::recursive_mutex eventLoopMutexAudio;

  public:

    void reloadAudioProfiles();
    void saveAudioProfiles();

	//TODO

    Q_INVOKABLE unsigned getAudioProfileCount();
    Q_INVOKABLE QString getAudioProfileName(unsigned index);

  public slots:
    void addAudioProfile(QString name);
    void applyAudioProfile(unsigned index);
    void deleteAudioProfile(unsigned index);

  signals:
    void audioProfilesUpdated();
  };

} // namespace advsettings
