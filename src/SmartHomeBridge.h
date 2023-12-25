#pragma once


#define paramDelay(time) (uint32_t)( \
            (time & 0xC000) == 0xC000 ? (time & 0x3FFF) * 100 : \
            (time & 0xC000) == 0x0000 ? (time & 0x3FFF) * 1000 : \
            (time & 0xC000) == 0x4000 ? (time & 0x3FFF) * 60000 : \
            (time & 0xC000) == 0x8000 ? ((time & 0x3FFF) > 1000 ? 3600000 : \
                                         (time & 0x3FFF) * 3600000 ) : 0 )
                                             
#define MAIN_OpenKnxId 0xAE
#define MAIN_ApplicationNumber 41
#define MAIN_ApplicationVersion 13
#define MAIN_ParameterSize 183
#define MAIN_MaxKoNumber 58
#define MAIN_OrderNumber "MGKnxBRI"
// Parameter with single occurrence


#define BASE_StartupDelayBase                     0      // 2 Bits, Bit 7-6
#define     BASE_StartupDelayBaseMask 0xC0
#define     BASE_StartupDelayBaseShift 6
#define BASE_StartupDelayTime                     0      // 14 Bits, Bit 13-0
#define     BASE_StartupDelayTimeMask 0x3FFF
#define     BASE_StartupDelayTimeShift 0
#define BASE_HeartbeatDelayBase                   2      // 2 Bits, Bit 7-6
#define     BASE_HeartbeatDelayBaseMask 0xC0
#define     BASE_HeartbeatDelayBaseShift 6
#define BASE_HeartbeatDelayTime                   2      // 14 Bits, Bit 13-0
#define     BASE_HeartbeatDelayTimeMask 0x3FFF
#define     BASE_HeartbeatDelayTimeShift 0
#define BASE_Timezone                             4      // 5 Bits, Bit 7-3
#define     BASE_TimezoneMask 0xF8
#define     BASE_TimezoneShift 3
#define BASE_TimezoneSign                         4      // 1 Bit, Bit 7
#define     BASE_TimezoneSignMask 0x80
#define     BASE_TimezoneSignShift 7
#define BASE_TimezoneValue                        4      // 4 Bits, Bit 6-3
#define     BASE_TimezoneValueMask 0x78
#define     BASE_TimezoneValueShift 3
#define BASE_CombinedTimeDate                     4      // 1 Bit, Bit 2
#define     BASE_CombinedTimeDateMask 0x04
#define     BASE_CombinedTimeDateShift 2
#define BASE_SummertimeAll                        4      // 2 Bits, Bit 1-0
#define     BASE_SummertimeAllMask 0x03
#define     BASE_SummertimeAllShift 0
#define BASE_SummertimeDE                         4      // 2 Bits, Bit 1-0
#define     BASE_SummertimeDEMask 0x03
#define     BASE_SummertimeDEShift 0
#define BASE_SummertimeWorld                      4      // 2 Bits, Bit 1-0
#define     BASE_SummertimeWorldMask 0x03
#define     BASE_SummertimeWorldShift 0
#define BASE_SummertimeKO                         4      // 2 Bits, Bit 1-0
#define     BASE_SummertimeKOMask 0x03
#define     BASE_SummertimeKOShift 0
#define BASE_Diagnose                             5      // 1 Bit, Bit 7
#define     BASE_DiagnoseMask 0x80
#define     BASE_DiagnoseShift 7
#define BASE_Watchdog                             5      // 1 Bit, Bit 6
#define     BASE_WatchdogMask 0x40
#define     BASE_WatchdogShift 6
#define BASE_ReadTimeDate                         5      // 1 Bit, Bit 5
#define     BASE_ReadTimeDateMask 0x20
#define     BASE_ReadTimeDateShift 5
#define BASE_HeartbeatExtended                    5      // 1 Bit, Bit 4
#define     BASE_HeartbeatExtendedMask 0x10
#define     BASE_HeartbeatExtendedShift 4
#define BASE_Latitude                             6      // float
#define BASE_Longitude                           10      // float

// Zeitbasis
#define ParamBASE_StartupDelayBase                    ((knx.paramByte(BASE_StartupDelayBase) & BASE_StartupDelayBaseMask) >> BASE_StartupDelayBaseShift)
// Zeit
#define ParamBASE_StartupDelayTime                    (knx.paramWord(BASE_StartupDelayTime) & BASE_StartupDelayTimeMask)
// Zeit (in Millisekunden)
#define ParamBASE_StartupDelayTimeMS                  (paramDelay(knx.paramWord(BASE_StartupDelayTime)))
// Zeitbasis
#define ParamBASE_HeartbeatDelayBase                  ((knx.paramByte(BASE_HeartbeatDelayBase) & BASE_HeartbeatDelayBaseMask) >> BASE_HeartbeatDelayBaseShift)
// Zeit
#define ParamBASE_HeartbeatDelayTime                  (knx.paramWord(BASE_HeartbeatDelayTime) & BASE_HeartbeatDelayTimeMask)
// Zeit (in Millisekunden)
#define ParamBASE_HeartbeatDelayTimeMS                (paramDelay(knx.paramWord(BASE_HeartbeatDelayTime)))
// Zeitzone
#define ParamBASE_Timezone                            ((knx.paramByte(BASE_Timezone) & BASE_TimezoneMask) >> BASE_TimezoneShift)
// Zeitzone-Vorzeichen
#define ParamBASE_TimezoneSign                        ((bool)(knx.paramByte(BASE_TimezoneSign) & BASE_TimezoneSignMask))
// Zeitzone-Wert
#define ParamBASE_TimezoneValue                       ((knx.paramByte(BASE_TimezoneValue) & BASE_TimezoneValueMask) >> BASE_TimezoneValueShift)
// Empfangen über
#define ParamBASE_CombinedTimeDate                    ((bool)(knx.paramByte(BASE_CombinedTimeDate) & BASE_CombinedTimeDateMask))
// Sommerzeit ermitteln durch
#define ParamBASE_SummertimeAll                       (knx.paramByte(BASE_SummertimeAll) & BASE_SummertimeAllMask)
// Sommerzeit ermitteln durch
#define ParamBASE_SummertimeDE                        (knx.paramByte(BASE_SummertimeDE) & BASE_SummertimeDEMask)
// Sommerzeit ermitteln durch
#define ParamBASE_SummertimeWorld                     (knx.paramByte(BASE_SummertimeWorld) & BASE_SummertimeWorldMask)
// Sommerzeit ermitteln durch
#define ParamBASE_SummertimeKO                        (knx.paramByte(BASE_SummertimeKO) & BASE_SummertimeKOMask)
// Diagnoseobjekt anzeigen
#define ParamBASE_Diagnose                            ((bool)(knx.paramByte(BASE_Diagnose) & BASE_DiagnoseMask))
// Watchdog aktivieren
#define ParamBASE_Watchdog                            ((bool)(knx.paramByte(BASE_Watchdog) & BASE_WatchdogMask))
// Bei Neustart vom Bus lesen
#define ParamBASE_ReadTimeDate                        ((bool)(knx.paramByte(BASE_ReadTimeDate) & BASE_ReadTimeDateMask))
// Erweitertes "In Betrieb"
#define ParamBASE_HeartbeatExtended                   ((bool)(knx.paramByte(BASE_HeartbeatExtended) & BASE_HeartbeatExtendedMask))
// Breitengrad
#define ParamBASE_Latitude                            (knx.paramFloat(BASE_Latitude, Float_Enc_IEEE754Single))
// Längengrad
#define ParamBASE_Longitude                           (knx.paramFloat(BASE_Longitude, Float_Enc_IEEE754Single))

#define BASE_KoHeartbeat 1
#define BASE_KoTime 2
#define BASE_KoDate 3
#define BASE_KoDiagnose 7
#define BASE_KoIsSummertime 10

// In Betrieb
#define KoBASE_Heartbeat                           (knx.getGroupObject(BASE_KoHeartbeat))
// Uhrzeit/Datum
#define KoBASE_Time                                (knx.getGroupObject(BASE_KoTime))
// Datum
#define KoBASE_Date                                (knx.getGroupObject(BASE_KoDate))
// Diagnose
#define KoBASE_Diagnose                            (knx.getGroupObject(BASE_KoDiagnose))
// Sommerzeit aktiv
#define KoBASE_IsSummertime                        (knx.getGroupObject(BASE_KoIsSummertime))

#define BRI_BridgeName                          18      // char*, 25 Byte
#define BRI_Modus                               43      // 8 Bits, Bit 7-0
#define BRI_WiFiSSID                            44      // char*, 32 Byte
#define BRI_WiFiPassword                        76      // char*, 63 Byte
#define BRI_PairingCode                         140      // char*, 8 Byte

// Name
#define ParamBRI_BridgeName                          (knx.paramData(BRI_BridgeName))
// Modus
#define ParamBRI_Modus                               (knx.paramByte(BRI_Modus))
// SSID
#define ParamBRI_WiFiSSID                            (knx.paramData(BRI_WiFiSSID))
// Password
#define ParamBRI_WiFiPassword                        (knx.paramData(BRI_WiFiPassword))
// Kopplungscode
#define ParamBRI_PairingCode                         (knx.paramData(BRI_PairingCode))

#define BRI_KoWLANState 20
#define BRI_KoHomeKitFactoryReset 21

// WLAN Status
#define KoBRI_WLANState                           (knx.getGroupObject(BRI_KoWLANState))
// Homekit Factory Reset
#define KoBRI_HomeKitFactoryReset                 (knx.getGroupObject(BRI_KoHomeKitFactoryReset))

#define BRI_ChannelCount 1

// Parameter per channel
#define BRI_ParamBlockOffset 148
#define BRI_ParamBlockSize 35
#define BRI_ParamCalcIndex(index) (index + BRI_ParamBlockOffset + _channelIndex * BRI_ParamBlockSize)

#define BRI_CHDeviceType                         0      // 8 Bits, Bit 7-0
#define BRI_CHDeviceName                         1      // char*, 25 Byte
#define BRI_CHDisableChannel                    26      // 1 Bit, Bit 7
#define     BRI_CHDisableChannelMask 0x80
#define     BRI_CHDisableChannelShift 7
#define BRI_CHDimmerSwitchOnBehavior            27      // 8 Bits, Bit 7-0
#define BRI_CHJalousieHueEmulation              27      // 8 Bits, Bit 7-0
#define BRI_CHRolladenHueEmulation              27      // 8 Bits, Bit 7-0
#define BRI_CHThermostatTemperaturUnitType      27      // 8 Bits, Bit 7-0
#define BRI_CHDisplayType                       27      // 8 Bits, Bit 7-0
#define BRI_CHContactAlarmSensorType            27      // 8 Bits, Bit 7-0
#define BRI_CHFanHueEmulation                   27      // 8 Bits, Bit 7-0
#define BRI_CHDoorWindowType                    27      // 8 Bits, Bit 7-0
#define BRI_CHDimmerSwitchOn2Behavior           28      // 8 Bits, Bit 7-0
#define BRI_CHJalousieUpDownHandling            28      // 8 Bits, Bit 7-0
#define BRI_CHRolladenUpDownHandling            28      // 8 Bits, Bit 7-0
#define BRI_CHThermostatMode                    28      // 8 Bits, Bit 7-0
#define BRI_CHContactAlarmSensorInvert          28      // 8 Bits, Bit 7-0
#define BRI_CHFanAutomatic                      28      // 8 Bits, Bit 7-0
#define BRI_CHDoorWindowMotor                   28      // 8 Bits, Bit 7-0
#define BRI_CHJalousieUseStop                   29      // 8 Bits, Bit 7-0
#define BRI_CHRolladenUseStop                   29      // 8 Bits, Bit 7-0
#define BRI_CHThermostatKoModeHeating           29      // 8 Bits, Bit 7-0
#define BRI_CHFanKoAutomatic                    29      // 8 Bits, Bit 7-0
#define BRI_CHDoorWindowObstructionDetection    29      // 8 Bits, Bit 7-0
#define BRI_CHSlatHandling                      30      // 8 Bits, Bit 7-0
#define BRI_CHThermostatKoModeHeatingFeedback   30      // 8 Bits, Bit 7-0
#define BRI_CHFanKoAutomaticFeedback            30      // 8 Bits, Bit 7-0
#define BRI_CHDoorWindowUpDownHandling          30      // 8 Bits, Bit 7-0
#define BRI_CHThemostateHeatingFeedbackKoType   31      // 8 Bits, Bit 7-0
#define BRI_CHDoorWindowUseStop                 31      // 8 Bits, Bit 7-0
#define BRI_CHThermostatKoModeCooling           32      // 8 Bits, Bit 7-0
#define BRI_CHDoorWindowFeedbackType            32      // 8 Bits, Bit 7-0
#define BRI_CHThermostatKoModeCoolingFeedback   33      // 8 Bits, Bit 7-0
#define BRI_CHThemostateCoolingFeedbackKoType   34      // 8 Bits, Bit 7-0

// Gerät
#define ParamBRI_CHDeviceType                        (knx.paramByte(BRI_ParamCalcIndex(BRI_CHDeviceType)))
// Name
#define ParamBRI_CHDeviceName                        (knx.paramData(BRI_ParamCalcIndex(BRI_CHDeviceName)))
// Gerät deaktivieren (Konfiguration bleibt erhalten)
#define ParamBRI_CHDisableChannel                    ((bool)(knx.paramByte(BRI_ParamCalcIndex(BRI_CHDisableChannel)) & BRI_CHDisableChannelMask))
// Bei EIN Befehl
#define ParamBRI_CHDimmerSwitchOnBehavior            (knx.paramByte(BRI_ParamCalcIndex(BRI_CHDimmerSwitchOnBehavior)))
// Jalousie in HUE als dimmbare Lampe darstellen
#define ParamBRI_CHJalousieHueEmulation              (knx.paramByte(BRI_ParamCalcIndex(BRI_CHJalousieHueEmulation)))
// Rolladen/Markise in HUE als dimmbare Lampe darstellen
#define ParamBRI_CHRolladenHueEmulation              (knx.paramByte(BRI_ParamCalcIndex(BRI_CHRolladenHueEmulation)))
// Einheit
#define ParamBRI_CHThermostatTemperaturUnitType      (knx.paramByte(BRI_ParamCalcIndex(BRI_CHThermostatTemperaturUnitType)))
// Anzeige
#define ParamBRI_CHDisplayType                       (knx.paramByte(BRI_ParamCalcIndex(BRI_CHDisplayType)))
// Sensor
#define ParamBRI_CHContactAlarmSensorType            (knx.paramByte(BRI_ParamCalcIndex(BRI_CHContactAlarmSensorType)))
// Lüfter in HUE als Lampe darstellen
#define ParamBRI_CHFanHueEmulation                   (knx.paramByte(BRI_ParamCalcIndex(BRI_CHFanHueEmulation)))
// Art
#define ParamBRI_CHDoorWindowType                    (knx.paramByte(BRI_ParamCalcIndex(BRI_CHDoorWindowType)))
// Bei EIN wenn aktuelle Helligkeit > 0%
#define ParamBRI_CHDimmerSwitchOn2Behavior           (knx.paramByte(BRI_ParamCalcIndex(BRI_CHDimmerSwitchOn2Behavior)))
// Auf/Ab Objekt verwenden
#define ParamBRI_CHJalousieUpDownHandling            (knx.paramByte(BRI_ParamCalcIndex(BRI_CHJalousieUpDownHandling)))
// Auf/Ab Objekt verwenden
#define ParamBRI_CHRolladenUpDownHandling            (knx.paramByte(BRI_ParamCalcIndex(BRI_CHRolladenUpDownHandling)))
// Optionen
#define ParamBRI_CHThermostatMode                    (knx.paramByte(BRI_ParamCalcIndex(BRI_CHThermostatMode)))
// Eingang Invertieren
#define ParamBRI_CHContactAlarmSensorInvert          (knx.paramByte(BRI_ParamCalcIndex(BRI_CHContactAlarmSensorInvert)))
// Lüfter hat Automatikmodus
#define ParamBRI_CHFanAutomatic                      (knx.paramByte(BRI_ParamCalcIndex(BRI_CHFanAutomatic)))
// Motorantrieb
#define ParamBRI_CHDoorWindowMotor                   (knx.paramByte(BRI_ParamCalcIndex(BRI_CHDoorWindowMotor)))
// Stop Objekt verwenden
#define ParamBRI_CHJalousieUseStop                   (knx.paramByte(BRI_ParamCalcIndex(BRI_CHJalousieUseStop)))
// Stop Objekt verwenden
#define ParamBRI_CHRolladenUseStop                   (knx.paramByte(BRI_ParamCalcIndex(BRI_CHRolladenUseStop)))
// Betriebsart
#define ParamBRI_CHThermostatKoModeHeating           (knx.paramByte(BRI_ParamCalcIndex(BRI_CHThermostatKoModeHeating)))
// Objekt für Automatik
#define ParamBRI_CHFanKoAutomatic                    (knx.paramByte(BRI_ParamCalcIndex(BRI_CHFanKoAutomatic)))
// Blockadenerkennung
#define ParamBRI_CHDoorWindowObstructionDetection    (knx.paramByte(BRI_ParamCalcIndex(BRI_CHDoorWindowObstructionDetection)))
// Lamellenposition bei Fahrt
#define ParamBRI_CHSlatHandling                      (knx.paramByte(BRI_ParamCalcIndex(BRI_CHSlatHandling)))
// Betriebsart Rückmeldung
#define ParamBRI_CHThermostatKoModeHeatingFeedback   (knx.paramByte(BRI_ParamCalcIndex(BRI_CHThermostatKoModeHeatingFeedback)))
// Objekt für Rückmeldung
#define ParamBRI_CHFanKoAutomaticFeedback            (knx.paramByte(BRI_ParamCalcIndex(BRI_CHFanKoAutomaticFeedback)))
// Auf/Ab Objekt verwenden
#define ParamBRI_CHDoorWindowUpDownHandling          (knx.paramByte(BRI_ParamCalcIndex(BRI_CHDoorWindowUpDownHandling)))
// Heizen aktiv Rückmeldung
#define ParamBRI_CHThemostateHeatingFeedbackKoType   (knx.paramByte(BRI_ParamCalcIndex(BRI_CHThemostateHeatingFeedbackKoType)))
// Stop Objekt verwenden
#define ParamBRI_CHDoorWindowUseStop                 (knx.paramByte(BRI_ParamCalcIndex(BRI_CHDoorWindowUseStop)))
// Betriebsart
#define ParamBRI_CHThermostatKoModeCooling           (knx.paramByte(BRI_ParamCalcIndex(BRI_CHThermostatKoModeCooling)))
// Rückmeldung
#define ParamBRI_CHDoorWindowFeedbackType            (knx.paramByte(BRI_ParamCalcIndex(BRI_CHDoorWindowFeedbackType)))
// Betriebsart Rückmeldung
#define ParamBRI_CHThermostatKoModeCoolingFeedback   (knx.paramByte(BRI_ParamCalcIndex(BRI_CHThermostatKoModeCoolingFeedback)))
// Kühlen aktiv Rückmeldung
#define ParamBRI_CHThemostateCoolingFeedbackKoType   (knx.paramByte(BRI_ParamCalcIndex(BRI_CHThemostateCoolingFeedbackKoType)))

// deprecated
#define BRI_KoOffset 50

// Communication objects per channel (multiple occurrence)
#define BRI_KoBlockOffset 50
#define BRI_KoBlockSize 9

#define BRI_KoCalcNumber(index) (index + BRI_KoBlockOffset + _channelIndex * BRI_KoBlockSize)
#define BRI_KoCalcIndex(number) ((number >= BRI_KoCalcNumber(0) && number < BRI_KoCalcNumber(BRI_KoBlockSize)) ? (number - BRI_KoOffset) % BRI_KoBlockSize : -1)

#define BRI_KoKO1_ 0
#define BRI_KoKO2_ 1
#define BRI_KoKO3_ 2
#define BRI_KoKO4_ 3
#define BRI_KoKO5_ 4
#define BRI_KoKO6_ 5
#define BRI_KoKO7_ 6
#define BRI_KoKO8_ 7
#define BRI_KoKO9_ 8

// KO1 %C%
#define KoBRI_KO1_                                (knx.getGroupObject(BRI_KoCalcNumber(BRI_KoKO1_)))
// KO2 %C%
#define KoBRI_KO2_                                (knx.getGroupObject(BRI_KoCalcNumber(BRI_KoKO2_)))
// KO3 %C%
#define KoBRI_KO3_                                (knx.getGroupObject(BRI_KoCalcNumber(BRI_KoKO3_)))
// KO4 %C%
#define KoBRI_KO4_                                (knx.getGroupObject(BRI_KoCalcNumber(BRI_KoKO4_)))
// KO5 %C%
#define KoBRI_KO5_                                (knx.getGroupObject(BRI_KoCalcNumber(BRI_KoKO5_)))
// KO6 %C%
#define KoBRI_KO6_                                (knx.getGroupObject(BRI_KoCalcNumber(BRI_KoKO6_)))
// KO7 %C%
#define KoBRI_KO7_                                (knx.getGroupObject(BRI_KoCalcNumber(BRI_KoKO7_)))
// KO8 %C%
#define KoBRI_KO8_                                (knx.getGroupObject(BRI_KoCalcNumber(BRI_KoKO8_)))
// KO9 %C%
#define KoBRI_KO9_                                (knx.getGroupObject(BRI_KoCalcNumber(BRI_KoKO9_)))



// Header generation for Module 'BASE_KommentarModule'

#define BASE_KommentarModuleCount 0
#define BASE_KommentarModuleModuleParamSize 0
#define BASE_KommentarModuleSubmodulesParamSize 0
#define BASE_KommentarModuleParamSize 0
#define BASE_KommentarModuleParamOffset 183
#define BASE_KommentarModuleCalcIndex(index, m1) (index + BASE_KommentarModuleParamOffset + _channelIndex * BASE_KommentarModuleCount * BASE_KommentarModuleParamSize + m1 * BASE_KommentarModuleParamSize)



