#include "ChaperoneTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings {


void ChaperoneTabController::initStage1() {
	m_height = getBoundsMaxY();
	reloadChaperoneProfiles();
	eventLoopTick();
}


void ChaperoneTabController::initStage2(OverlayController * parent, QQuickWindow * widget) {
	this->parent = parent;
	this->widget = widget;
}



void ChaperoneTabController::reloadChaperoneProfiles() {
	chaperoneProfiles.clear();
	auto settings = OverlayController::appSettings();
	settings->beginGroup("chaperoneSettings");
	auto profileCount = settings->beginReadArray("chaperoneProfiles");
	for (int i = 0; i < profileCount; i++) {
		settings->setArrayIndex(i);
		chaperoneProfiles.emplace_back();
		auto& entry = chaperoneProfiles[i];
		entry.profileName = settings->value("profileName").toString().toStdString();
		entry.includesChaperoneGeometry = settings->value("includesChaperoneGeometry", false).toBool();
		if (entry.includesChaperoneGeometry) {
			entry.chaperoneGeometryQuadCount = settings->value("chaperoneGeometryQuadCount", 0).toUInt();
			entry.chaperoneGeometryQuads.reset(new vr::HmdQuad_t[entry.chaperoneGeometryQuadCount]);
			unsigned qi = 0;
			for (auto q : settings->value("chaperoneGeometryQuads").toList()) {
				unsigned ci = 0;
				for (auto c : q.toList()) {
					unsigned cci = 0;
					for (auto cc : c.toList()) {
						entry.chaperoneGeometryQuads.get()[qi].vCorners[ci].v[cci] = cc.toFloat();
						cci++;
					}
					ci++;
				}
				qi++;
			}
			unsigned i1 = 0;
			for (auto l1 : settings->value("standingCenter").toList()) {
				unsigned i2 = 0;
				for (auto l2 : l1.toList()) {
					entry.standingCenter.m[i1][i2] = l2.toFloat();
					i2++;
				}
				i1++;
			}
			entry.playSpaceAreaX = settings->value("playSpaceAreaX", 0.0f).toFloat();
			entry.playSpaceAreaZ = settings->value("playSpaceAreaZ", 0.0f).toFloat();
		}
		entry.includesVisibility = settings->value("includesVisibility", false).toBool();
		if (entry.includesVisibility) {
			entry.visibility = settings->value("visibility", 0.6).toFloat();
		}
		entry.includesFadeDistance = settings->value("includesFadeDistance", false).toBool();
		if (entry.includesFadeDistance) {
			entry.fadeDistance = settings->value("fadeDistance", 0.7).toFloat();
		}
		entry.includesCenterMarker = settings->value("includesCenterMarker", false).toBool();
		if (entry.includesCenterMarker) {
			entry.centerMarker = settings->value("centerMarker", false).toBool();
		}
		entry.includesPlaySpaceMarker = settings->value("includesPlaySpaceMarker", false).toBool();
		if (entry.includesPlaySpaceMarker) {
			entry.playSpaceMarker = settings->value("playSpaceMarker", false).toBool();
		}
		entry.includesFloorBoundsMarker = settings->value("includesFloorBoundsMarker", false).toBool();
		if (entry.includesFloorBoundsMarker) {
			entry.floorBoundsMarker = settings->value("floorBoundsMarker", false).toBool();
		}
		entry.includesBoundsColor = settings->value("includesBoundsColor", false).toBool();
		if (entry.includesBoundsColor) {
			auto color = settings->value("boundsColor").toList();
			entry.boundsColor[0] = color[0].toInt();
			entry.boundsColor[1] = color[1].toInt();
			entry.boundsColor[2] = color[2].toInt();
		}
		entry.includesChaperoneStyle = settings->value("includesChaperoneStyle", false).toBool();
		if (entry.includesChaperoneStyle) {
			entry.chaperoneStyle = settings->value("chaperoneStyle", 0).toInt();
		}
		entry.includesForceBounds = settings->value("includesForceBounds", false).toBool();
		if (entry.includesForceBounds) {
			entry.forceBounds = settings->value("forceBounds", false).toBool();
		}
	}
	settings->endArray();
	settings->endGroup();
}

void ChaperoneTabController::saveChaperoneProfiles() {
	auto settings = OverlayController::appSettings();
	settings->beginGroup("chaperoneSettings");
	settings->beginWriteArray("chaperoneProfiles");
	unsigned i = 0;
	for (auto& p : chaperoneProfiles) {
		settings->setArrayIndex(i);
		settings->setValue("profileName", QString::fromStdString(p.profileName));
		settings->setValue("includesChaperoneGeometry", p.includesChaperoneGeometry);
		if (p.includesChaperoneGeometry) {
			settings->setValue("chaperoneGeometryQuadCount", p.chaperoneGeometryQuadCount);
			QVariantList quadList;
			for (unsigned qi = 0; qi < p.chaperoneGeometryQuadCount; qi++) {
				QVariantList cornerList;
				for (unsigned ci = 0; ci < 4; ci++) {
					QVariantList coordVector;
					for (unsigned cci = 0; cci < 3; cci++) {
						coordVector.push_back(p.chaperoneGeometryQuads.get()[qi].vCorners[ci].v[cci]);
					}
					cornerList.push_back(coordVector);
				}
				quadList.push_back(cornerList);
			}
			settings->setValue("chaperoneGeometryQuads", quadList);
			QVariantList l1;
			for (unsigned i1 = 0; i1 < 3; i1++) {
				QVariantList l2;
				for (unsigned i2 = 0; i2 < 4; i2++) {
					l2.push_back(p.standingCenter.m[i1][i2]);
				}
				l1.push_back(l2);
			}
			settings->setValue("standingCenter", l1);
			settings->setValue("playSpaceAreaX", p.playSpaceAreaX);
			settings->setValue("playSpaceAreaZ", p.playSpaceAreaZ);
		}
		settings->setValue("includesVisibility", p.includesVisibility);
		if (p.includesVisibility) {
			settings->setValue("visibility", p.visibility);
		}
		settings->setValue("includesFadeDistance", p.includesFadeDistance);
		if (p.includesFadeDistance) {
			settings->setValue("fadeDistance", p.fadeDistance);
		}
		settings->setValue("includesCenterMarker", p.includesCenterMarker);
		if (p.includesCenterMarker) {
			settings->setValue("centerMarker", p.centerMarker);
		}
		settings->setValue("includesPlaySpaceMarker", p.includesPlaySpaceMarker);
		if (p.includesPlaySpaceMarker) {
			settings->setValue("playSpaceMarker", p.playSpaceMarker);
		}
		settings->setValue("includesFloorBoundsMarker", p.includesFloorBoundsMarker);
		if (p.includesFloorBoundsMarker) {
			settings->setValue("floorBoundsMarker", p.floorBoundsMarker);
		}
		settings->setValue("includesBoundsColor", p.includesBoundsColor);
		if (p.includesBoundsColor) {
			QVariantList color;
			color.push_back(p.boundsColor[0]);
			color.push_back(p.boundsColor[1]);
			color.push_back(p.boundsColor[2]);
			settings->setValue("boundsColor", color);
		}
		settings->setValue("includesChaperoneStyle", p.includesChaperoneStyle);
		if (p.includesChaperoneStyle) {
			settings->setValue("chaperoneStyle", p.chaperoneStyle);
		}
		settings->setValue("includesForceBounds", p.includesForceBounds);
		if (p.includesForceBounds) {
			settings->setValue("forceBounds", p.forceBounds);
		}
		i++;
	}
	settings->endArray();
	settings->endGroup();

}


void ChaperoneTabController::eventLoopTick() {
	if (settingsUpdateCounter >= 50) {
		vr::EVRSettingsError vrSettingsError;
		float vis = vr::VRSettings()->GetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaA_Int32, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_CollisionBounds_ColorGammaA_Int32 << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
		setBoundsVisibility(vis/255.0f);
		auto fd = vr::VRSettings()->GetFloat(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_FadeDistance_Float, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_CollisionBounds_FadeDistance_Float << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
		setFadeDistance(fd);
		auto cm = vr::VRSettings()->GetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_CenterMarkerOn_Bool, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_CollisionBounds_CenterMarkerOn_Bool << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
		setCenterMarker(cm);
		auto ps = vr::VRSettings()->GetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_PlaySpaceOn_Bool, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_CollisionBounds_PlaySpaceOn_Bool << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
		setPlaySpaceMarker(ps);
		settingsUpdateCounter = 0;
	} else {
		settingsUpdateCounter++;
	}
}


float ChaperoneTabController::boundsVisibility() const {
	return m_visibility;
}


void ChaperoneTabController::setBoundsVisibility(float value, bool notify) {
	if (m_visibility != value) {
		m_visibility = value;
		vr::VRSettings()->SetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaA_Int32, 255 * m_visibility);
		vr::VRSettings()->Sync();
		if (notify) {
			emit boundsVisibilityChanged(m_visibility);
		}
	}
}


float ChaperoneTabController::fadeDistance() const {
	return m_fadeDistance;
}


void ChaperoneTabController::setFadeDistance(float value, bool notify) {
	if (m_fadeDistance != value) {
		m_fadeDistance = value;
		vr::VRSettings()->SetFloat(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_FadeDistance_Float, m_fadeDistance);
		vr::VRSettings()->Sync();
		if (notify) {
			emit fadeDistanceChanged(m_fadeDistance);
		}
	}
}


float ChaperoneTabController::height() const {
	return m_height;
}


void ChaperoneTabController::setHeight(float value, bool notify) {
	if (m_height != value) {
		m_height = value;
		vr::VRChaperoneSetup()->RevertWorkingCopy();
		unsigned collisionBoundsCount = 0;
		vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(nullptr, &collisionBoundsCount);
		if (collisionBoundsCount > 0) {
			vr::HmdQuad_t* collisionBounds = new vr::HmdQuad_t[collisionBoundsCount];
			vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(collisionBounds, &collisionBoundsCount);
			for (unsigned b = 0; b < collisionBoundsCount; b++) {
				collisionBounds[b].vCorners[0].v[1] = 0.0;
				collisionBounds[b].vCorners[1].v[1] = value;
				collisionBounds[b].vCorners[2].v[1] = value;
				collisionBounds[b].vCorners[3].v[1] = 0.0;
			}
			vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(collisionBounds, collisionBoundsCount);
			vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
			delete collisionBounds;
		}
		if (notify) {
			emit heightChanged(m_fadeDistance);
		}
	}
}


void ChaperoneTabController::updateHeight(float value, bool notify) {
	if (m_height != value) {
		m_height = value;
		if (notify) {
			emit heightChanged(m_fadeDistance);
		}
	}
}


bool ChaperoneTabController::centerMarker() const {
	return m_centerMarker;
}

void ChaperoneTabController::setCenterMarker(bool value, bool notify) {
	if (m_centerMarker != value) {
		m_centerMarker = value;
		vr::VRSettings()->SetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_CenterMarkerOn_Bool, m_centerMarker);
		vr::VRSettings()->Sync();
		if (notify) {
			emit centerMarkerChanged(m_centerMarker);
		}
	}
}

bool ChaperoneTabController::playSpaceMarker() const {
	return m_playSpaceMarker;
}

void ChaperoneTabController::setPlaySpaceMarker(bool value, bool notify) {
	if (m_playSpaceMarker != value) {
		m_playSpaceMarker = value;
		vr::VRSettings()->SetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_PlaySpaceOn_Bool, m_playSpaceMarker);
		vr::VRSettings()->Sync();
		if (notify) {
			emit playSpaceMarkerChanged(m_playSpaceMarker);
		}
	}
}

bool ChaperoneTabController::forceBounds() const {
	return m_forceBounds;
}

Q_INVOKABLE unsigned ChaperoneTabController::getChaperoneProfileCount() {
	return (unsigned)chaperoneProfiles.size();
}

Q_INVOKABLE QString ChaperoneTabController::getChaperoneProfileName(unsigned index) {
	if (index >= chaperoneProfiles.size()) {
		return QString();
	} else {
		return QString::fromStdString(chaperoneProfiles[index].profileName);
	}
}

void ChaperoneTabController::setForceBounds(bool value, bool notify) {
	if (m_forceBounds != value) {
		m_forceBounds = value;
		vr::VRChaperone()->ForceBoundsVisible(m_forceBounds);
		if (notify) {
			emit forceBoundsChanged(m_forceBounds);
		}
	}
}

void ChaperoneTabController::flipOrientation() {
	parent->RotateUniverseCenter(vr::TrackingUniverseStanding, (float)M_PI);
}

void ChaperoneTabController::reloadFromDisk() {
	vr::VRChaperoneSetup()->ReloadFromDisk(vr::EChaperoneConfigFile_Live);
	vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
}

void ChaperoneTabController::addChaperoneProfile(QString name, bool includeGeometry, bool includeVisbility, bool includeFadeDistance, bool includeCenterMarker,
		bool includePlaySpaceMarker, bool includeFloorBounds, bool includeBoundsColor, bool includeChaperoneStyle, bool includeForceBounds) {
	vr::EVRSettingsError vrSettingsError;
	auto i = chaperoneProfiles.size();
	chaperoneProfiles.emplace_back();
	auto& profile = chaperoneProfiles[i];
	profile.profileName = name.toStdString();
	profile.includesChaperoneGeometry = includeGeometry;
	if (includeGeometry) {
		vr::VRChaperoneSetup()->RevertWorkingCopy();
		uint32_t quadCount = 0;
		vr::VRChaperoneSetup()->GetLiveCollisionBoundsInfo(nullptr, &quadCount);
		profile.chaperoneGeometryQuadCount = quadCount;
		profile.chaperoneGeometryQuads.reset(new vr::HmdQuad_t[quadCount]);
		vr::VRChaperoneSetup()->GetLiveCollisionBoundsInfo(profile.chaperoneGeometryQuads.get(), &quadCount);
		vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(&profile.standingCenter);
		vr::VRChaperoneSetup()->GetWorkingPlayAreaSize(&profile.playSpaceAreaX, &profile.playSpaceAreaZ);
	}
	profile.includesVisibility = includeVisbility;
	if (includeVisbility) {
		profile.visibility = m_visibility;
	}
	profile.includesFadeDistance = includeFadeDistance;
	if (includeFadeDistance) {
		profile.fadeDistance = m_fadeDistance;
	}
	profile.includesCenterMarker = includeCenterMarker;
	if (includeCenterMarker) {
		profile.centerMarker = m_centerMarker;
	}
	profile.includesPlaySpaceMarker = includePlaySpaceMarker;
	if (includePlaySpaceMarker) {
		profile.playSpaceMarker = m_playSpaceMarker;
	}
	profile.includesFloorBoundsMarker = includeFloorBounds;
	if (includeFloorBounds) {
		profile.floorBoundsMarker = vr::VRSettings()->GetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_GroundPerimeterOn_Bool, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_CollisionBounds_GroundPerimeterOn_Bool << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
	}
	profile.includesBoundsColor = includeBoundsColor;
	if (includeBoundsColor) {
		profile.boundsColor[0] = vr::VRSettings()->GetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaR_Int32, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_CollisionBounds_ColorGammaR_Int32 << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
		profile.boundsColor[1] = vr::VRSettings()->GetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaG_Int32, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_CollisionBounds_ColorGammaG_Int32 << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
		profile.boundsColor[2] = vr::VRSettings()->GetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaB_Int32, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_CollisionBounds_ColorGammaB_Int32 << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
	}
	profile.includesChaperoneStyle = includeChaperoneStyle;
	if (includeChaperoneStyle) {
		profile.chaperoneStyle = vr::VRSettings()->GetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_Style_Int32, &vrSettingsError);
		if (vrSettingsError != vr::VRSettingsError_None) {
			LOG(WARNING) << "Could not read \"" << vr::k_pch_CollisionBounds_Style_Int32 << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
		}
	}
	profile.includesForceBounds = includeForceBounds;
	if (includeForceBounds) {
		profile.forceBounds = m_forceBounds;
	}
	saveChaperoneProfiles();
	OverlayController::appSettings()->sync();
	emit chaperoneProfilesUpdated();
}

void ChaperoneTabController::applyChaperoneProfile(unsigned index) {
	if (index < chaperoneProfiles.size()) {
		auto& profile = chaperoneProfiles[index];
		if (profile.includesChaperoneGeometry) {
			vr::VRChaperoneSetup()->RevertWorkingCopy();
			vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(profile.chaperoneGeometryQuads.get(), profile.chaperoneGeometryQuadCount);
			vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(&profile.standingCenter);
			vr::VRChaperoneSetup()->SetWorkingPlayAreaSize(profile.playSpaceAreaX, profile.playSpaceAreaZ);
			vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
		}
		if (profile.includesVisibility) {
			setBoundsVisibility(profile.visibility);
		}
		if (profile.includesFadeDistance) {
			setFadeDistance(profile.fadeDistance);
		}
		if (profile.includesCenterMarker) {
			setCenterMarker(profile.centerMarker);
		}
		if (profile.includesPlaySpaceMarker) {
			setPlaySpaceMarker(profile.playSpaceMarker);
		}
		if (profile.includesFloorBoundsMarker) {
			vr::VRSettings()->SetBool(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_GroundPerimeterOn_Bool, profile.floorBoundsMarker);
		}
		if (profile.includesBoundsColor) {
			vr::VRSettings()->SetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaR_Int32, profile.boundsColor[0]);
			vr::VRSettings()->SetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaG_Int32, profile.boundsColor[1]);
			vr::VRSettings()->SetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaB_Int32, profile.boundsColor[2]);
		}
		if (profile.includesChaperoneStyle) {
			vr::VRSettings()->SetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_Style_Int32, profile.chaperoneStyle);
		}
		if (profile.includesForceBounds) {
			setForceBounds(profile.forceBounds);
		}
		vr::VRSettings()->Sync();
		updateHeight(getBoundsMaxY());
	}
}

void ChaperoneTabController::deleteChaperoneProfile(unsigned index) {
	if (index < chaperoneProfiles.size()) {
		auto pos = chaperoneProfiles.begin() + index;
		chaperoneProfiles.erase(pos);
		saveChaperoneProfiles();
		OverlayController::appSettings()->sync();
		emit chaperoneProfilesUpdated();
	}
}

float ChaperoneTabController::getBoundsMaxY() {
	unsigned collisionBoundsCount = 0;
	float result = FP_NAN;
	vr::VRChaperoneSetup()->GetLiveCollisionBoundsInfo(nullptr, &collisionBoundsCount);
	if (collisionBoundsCount > 0) {
		vr::HmdQuad_t* collisionBounds = new vr::HmdQuad_t[collisionBoundsCount];
		vr::VRChaperoneSetup()->GetLiveCollisionBoundsInfo(collisionBounds, &collisionBoundsCount);
		for (unsigned b = 0; b < collisionBoundsCount; b++) {
			int ci;
			if (collisionBounds[b].vCorners[1].v[1] >= collisionBounds[b].vCorners[2].v[1]) {
				ci = 1;
			} else {
				ci = 2;
			}
			if (isnan(result) || result < collisionBounds[b].vCorners[ci].v[1]) {
				result = collisionBounds[b].vCorners[ci].v[1];
			}
		}
		delete collisionBounds;
	}
	return result;
}

void ChaperoneTabController::reset() {
	vr::EVRSettingsError vrSettingsError;

	vr::VRSettings()->RemoveKeyInSection(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaA_Int32, &vrSettingsError);
	if (vrSettingsError != vr::VRSettingsError_None) {
		LOG(WARNING) << "Could not remove \"" << vr::k_pch_CollisionBounds_ColorGammaA_Int32 << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
	}

	vr::VRSettings()->RemoveKeyInSection(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_FadeDistance_Float, &vrSettingsError);
	if (vrSettingsError != vr::VRSettingsError_None) {
		LOG(WARNING) << "Could not remove \"" << vr::k_pch_CollisionBounds_FadeDistance_Float << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
	}

	vr::VRSettings()->RemoveKeyInSection(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_CenterMarkerOn_Bool, &vrSettingsError);
	if (vrSettingsError != vr::VRSettingsError_None) {
		LOG(WARNING) << "Could not remove \"" << vr::k_pch_CollisionBounds_CenterMarkerOn_Bool << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
	}

	vr::VRSettings()->RemoveKeyInSection(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_PlaySpaceOn_Bool, &vrSettingsError);
	if (vrSettingsError != vr::VRSettingsError_None) {
		LOG(WARNING) << "Could not remove \"" << vr::k_pch_CollisionBounds_PlaySpaceOn_Bool << "\" setting: " << vr::VRSettings()->GetSettingsErrorNameFromEnum(vrSettingsError);
	}

	setForceBounds(false);

	vr::VRSettings()->Sync();
	settingsUpdateCounter = 999; // Easiest way to get default values
}

} // namespace advconfig
