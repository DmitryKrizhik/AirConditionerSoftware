#ifndef SIMULATORSOFTWARE_H
#define SIMULATORSOFTWARE_H

#include <QMainWindow>
#include "simulatorutils.h"
QT_BEGIN_NAMESPACE
namespace Ui { class SimulatorSoftware; }
QT_END_NAMESPACE

class SimulatorSoftware : public QMainWindow
{
    Q_OBJECT

public:
    SimulatorSoftware(QWidget *parent = nullptr);
    ~SimulatorSoftware();

private slots:
    void on_ThemeSelectionBox_activated(int newThemeType);
    void SetLightTheme();
    void SetDarkTheme();
    void on_ResolutionSelectionBox_activated(int newResolutionType);
    void on_CloseWindowbutton_clicked();
    void on_PressureUnitSelectionBox_activated(int newPressureUnit);
    void on_PowerButton_clicked();
    void StartSendingRequests();
    void SendRequest();
    void TurnONSimulatorSoftware();
    void TurnOFFSimulatorSoftware();
    void UpdateParametersInLabels();
    void UpdateTemperatureParametersInLabel();
    void UpdatePressureParametersInLabel();
    void UpdateAngleParametersInLabel();
    void UpdateHumidityParametersInLabel();
    void SetEnabledTrue();
    void SetEnabledFalse();
    void HideText();
    void UnhideText();
    void SetColorOfThePowerButton();
    void SetPowerButtonText();
    void on_TemperatureUnitSelectionBox_activated(int newTemperatureUnit);
    void UpdatePowerStateParameterInLabel();

private:
    Ui::SimulatorSoftware *ui;
};
#endif // SIMULATORSOFTWARE_H
