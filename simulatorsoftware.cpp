#include "simulatorsoftware.h"
#include "ui_simulatorsoftware.h"

SimulatorSoftware::SimulatorSoftware(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SimulatorSoftware)
{
    ui->setupUi(this);
    TurnONSimulatorSoftware();
    SetLightTheme();
}

SimulatorSoftware::~SimulatorSoftware()
{
    delete ui;
}

void SimulatorSoftware::SetLightTheme()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(Constants::rValueWindowLightTheme, Constants::gValueWindowLightTheme,
                                          Constants::bValueWindowLightTheme));
    pal.setColor(QPalette::Button, QColor(Constants::rValueButtonLightTheme, Constants::gValueButtonLightTheme,
                                          Constants::bValueButtonLightTheme));
    pal.setColor(QPalette::ButtonText, QColor(Constants::rValueWindowText, Constants::gValueWindowText,
                                              Constants::bValueWindowText));
    pal.setColor(QPalette::WindowText, QColor(Constants::rValueButtonText, Constants::gValueButtonText,
                                              Constants::bValueButtonText));
    setPalette(pal);
    SetColorOfThePowerButton();
}

void SimulatorSoftware::SetDarkTheme()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(Constants::rValueWindowDarkTheme, Constants::gValueWindowDarkTheme,
                                          Constants::bValueWindowDarkTheme));
    pal.setColor(QPalette::Button, QColor(Constants::rValueButtonDarkTheme, Constants::gValueButtonDarkTheme,
                                          Constants::bValueButtonDarkTheme));
    pal.setColor(QPalette::ButtonText, QColor(Constants::rValueWindowText, Constants::gValueWindowText,
                                              Constants::bValueWindowText));
    pal.setColor(QPalette::WindowText, QColor(Constants::rValueButtonText, Constants::gValueButtonText,
                                              Constants::bValueButtonText));
    setPalette(pal);
    SetColorOfThePowerButton();
}

void SimulatorSoftware::on_ThemeSelectionBox_activated(int ThemeType)
{

    switch(ThemeType)
    {
        case LIGHTTHEME:
            SimUtils.SetTheme(LIGHTTHEME);
            SetLightTheme();
            break;
        case DARKTHEME:
            SimUtils.SetTheme(DARKTHEME);
            SetDarkTheme();
            break;
    }
}

void SimulatorSoftware::on_ResolutionSelectionBox_activated(int newResolutionType)
{
    switch(newResolutionType)
    {
        case RESOLUTION800x600:
            setFixedSize(Constants::horizontalSmallResolitionValue,
                         Constants::verticalSmallResolitionValue); break;
        case RESOLUTION1024x768:
            setFixedSize(Constants::horizontalBigResolitionValue,
                         Constants::verticalBigResolitionValue); break;
         }
}

void SimulatorSoftware::on_CloseWindowbutton_clicked()
{
    qApp->quit();
}

void SimulatorSoftware::on_PressureUnitSelectionBox_activated(int newPressureType){ SimUtils.SetPressureUnitSoftware(newPressureType); }

void SimulatorSoftware::on_PowerButton_clicked()
{
    if(SimUtils.GetPowerState() == ON){ TurnOFFSimulatorSoftware(); }
    else{ TurnONSimulatorSoftware(); }
}
//void Simulator::TurnOnSimulator()
//{
//    SimUtils.ReadParametersFromFile();
//    Controller.ReadTCPSettingsFromFile();
//    qDebug() << Controller.GetHOST();
//    qDebug() << Controller.GetPORT();
//    qDebug() << Controller.GetRequestInterval();
//    SetEnabledTrue();
//    SetParametersToLabels();
//    SimUtils.SetPowerState(ON);
//    SetColorOfThePowerButton();
//    SetPowerButtonText();
//}

//void Simulator::TurnOffSimulator()
//{
//    SimUtils.WriteParametersInFile();
//    SetEnabledFalse();
//    SetParametersToLabels();
//    SimUtils.SetPowerState(OFF);
//    SetColorOfThePowerButton();
//    SetPowerButtonText();
//}
void SimulatorSoftware::TurnONSimulatorSoftware()
{
    Controller.ReadTCPSettingsFromFile();
    qDebug() << Controller.GetHOST();
    qDebug() << Controller.GetPORT();
    qDebug() << Controller.GetRequestInterval();
    StartSendingRequests();
    SetEnabledTrue();
    SimUtils.SetPowerState(ON);
    SetColorOfThePowerButton();
    SetPowerButtonText();
}

void SimulatorSoftware::TurnOFFSimulatorSoftware()
{
    SetEnabledFalse();
    SimUtils.SetPowerState(OFF);
    SetColorOfThePowerButton();
    SetPowerButtonText();
}

void SimulatorSoftware::StartSendingRequests()
{
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SimulatorSoftware::SendRequest);
    timer->start(Controller.GetRequestInterval());
}
void SimulatorSoftware::SendRequest()
{
        QString tmpStr = Controller.GetHOST();
        QHostAddress address(tmpStr);

        Controller.socket.connectToHost(address, Controller.GetPORT());

        if (Controller.socket.waitForConnected())
        {
            Controller.socket.write("GETDATA");
            Controller.socket.flush();
            Controller.socket.waitForBytesWritten();
            Controller.socket.waitForReadyRead();

            QByteArray response = Controller.socket.readAll();
            QDataStream in(&response, QIODevice::ReadOnly);
            qint16 tmpTemperatureValue;
            qint16 tmpTemperatureUnit;
            qint16 tmpPressureValue;
            qint16 tmpPressureUnit;
            qint16 tmpAngleValue;
            qint16 tmpHumidityValue;

            in >> tmpTemperatureValue >> tmpTemperatureUnit >> tmpPressureValue >> tmpPressureUnit >> tmpAngleValue
                    >> tmpHumidityValue;

            SimUtils.SetTemperatureValue(tmpTemperatureValue);
            SimUtils.SetTemperatureUnit(tmpTemperatureUnit);
            SimUtils.SetPressureValue(tmpPressureValue);
            SimUtils.SetPressureUnit(tmpPressureUnit);
            SimUtils.SetAngleValue(tmpAngleValue);
            SimUtils.SetHumidityValue(tmpHumidityValue);
            UpdateParametersInLabels();
            Controller.socket.close();
        }
}

void SimulatorSoftware::UpdateParametersInLabels()
{
    UpdateTemperatureParametersInLabel();
    UpdatePressureParametersInLabel();
    UpdateAngleParametersInLabel();
    UpdateHumidityParametersInLabel();
}

void SimulatorSoftware::on_TemperatureUnitSelectionBox_activated(int newTemperatureType){ SimUtils.SetTemperatureUnitSoftware(newTemperatureType); }

void SimulatorSoftware::UpdateTemperatureParametersInLabel()
{
    switch(SimUtils.GetTemperatureUnit())
    {
    case CELSIUS:
        switch(SimUtils.GetTemperatureUnitSoftware())
        {
            case CELSIUS:
                ui->TemperatureActualParametersLabel->setText("Temperature: " + QString::number(SimUtils.GetTemperatureValue()) + "° C"); break;
            case KELVIN:
                ui->TemperatureActualParametersLabel->setText("Temperature: " + QString::number(Converter.ConvertCelsiusToKelvins
                                                                              (SimUtils.GetTemperatureValue())) + " K"); break;
            case FHARENHEIT:
                ui->TemperatureActualParametersLabel->setText("Temperature: " + QString::number(Converter.ConvertCelsiusToFharenheit
                                                                              (SimUtils.GetTemperatureValue())) + "° F"); break;
        } break;
    case KELVIN:
        switch(SimUtils.GetTemperatureUnitSoftware())
        {
            case CELSIUS:
                ui->TemperatureActualParametersLabel->setText("Temperature: " + QString::number(Converter.ConvertKelvinsToCelsius
                                                                              (SimUtils.GetTemperatureValue())) + "° C"); break;
            case KELVIN:
                ui->TemperatureActualParametersLabel->setText("Temperature: " + QString::number(SimUtils.GetTemperatureValue()) + " K"); break;
            case FHARENHEIT:
                ui->TemperatureActualParametersLabel->setText("Temperature: " + QString::number(Converter.ConvertKelvinsToFharenheit
                                                                               (SimUtils.GetTemperatureValue())) + "° F"); break;
        } break;
    case FHARENHEIT:
        switch(SimUtils.GetTemperatureUnitSoftware())
        {
            case CELSIUS:
                ui->TemperatureActualParametersLabel->setText("Temperature: " + QString::number(Converter.ConvertFharenheitToCelsius
                                                                              (SimUtils.GetTemperatureValue())) + "° C"); break;
            case KELVIN:
                 ui->TemperatureActualParametersLabel->setText("Temperature: " + QString::number(Converter.ConvertFharenheitToKelvins
                                                                              (SimUtils.GetTemperatureValue())) + " K"); break;
            case FHARENHEIT:
                ui->TemperatureActualParametersLabel->setText("Temperature: " + QString::number(SimUtils.GetTemperatureValue()) + "° F"); break;
        } break;

    }
}

void SimulatorSoftware::UpdatePressureParametersInLabel()
{
    switch(SimUtils.GetPressureUnit())
    {
        case KILOPASKALS:
            switch(SimUtils.GetPressureUnitSoftware())
            {
            case KILOPASKALS:
                ui->PressureActualParametersLabel->setText("Pressure: " + QString::number(SimUtils.GetPressureValue()) + " KPa"); break;
            case MMHG:
                ui->PressureActualParametersLabel->setText("Pressure: " + QString::number(Converter.ConvertKilopaskalsToMMHG
                                                                           (SimUtils.GetPressureValue())) + " mmHg"); break;
            } break;
        case MMHG:
            switch(SimUtils.GetPressureUnitSoftware())
            {
            case KILOPASKALS:
                ui->PressureActualParametersLabel->setText("Pressure: " + QString::number(Converter.ConvertMMHGToKilopaskals
                                                                           (SimUtils.GetPressureValue())) + " KPa"); break;
            case MMHG:
                ui->PressureActualParametersLabel->setText("Pressure: " + QString::number(SimUtils.GetPressureValue()) + " mmHg"); break;
            }
    }
}

void SimulatorSoftware::UpdateAngleParametersInLabel()
{
    if(SimUtils.GetAngleValue() > ZERO)
    {
        ui->AngleActualParametersLabel->setText("Angle of rotation: " + QString::number(SimUtils.GetAngleValue()) + "° right");
    }
    else if(SimUtils.GetAngleValue() < ZERO)
    {
        ui->AngleActualParametersLabel->setText("Angle of rotation: " + QString::number(-SimUtils.GetAngleValue()) + "° left");
    }
    else{ ui->AngleActualParametersLabel->setText("Angle of rotation: " + QString::number(ZERO) + "°");}
}

void SimulatorSoftware::UpdateHumidityParametersInLabel(){ ui->HumidityActualParametersLabel->setText
            ("Humidity: " + QString::number(SimUtils.GetHumidityValue()) + "%"); }

void SimulatorSoftware::UpdatePowerStateParameterInLabel()
{

}
void SimulatorSoftware::SetEnabledTrue()
{
    ui->ResolutionSelectionBox->setEnabled(true);
    ui->ThemeSelectionBox->setEnabled(true);
    ui->PressureUnitSelectionBox->setEnabled(true);
    ui->TemperatureActualParametersLabel->setEnabled(true);
    UnhideText();
}
void SimulatorSoftware::SetEnabledFalse()
{
    ui->ResolutionSelectionBox->setEnabled(false);
    ui->ThemeSelectionBox->setEnabled(false);
    ui->PressureUnitSelectionBox->setEnabled(false);
    ui->TemperatureActualParametersLabel->setEnabled(false);
    HideText();
}

void SimulatorSoftware::HideText()
{
    if(SimUtils.GetTheme() == LIGHTTHEME)
    {
        QPalette textPalette = palette();
        textPalette.setColor(QPalette::ButtonText, QColor(Constants::rValueButtonLightTheme, Constants::gValueButtonLightTheme,
                                                  Constants::bValueButtonLightTheme));
        textPalette.setColor(QPalette::WindowText, QColor(Constants::rValueWindowLightTheme, Constants::gValueWindowLightTheme,
                                                  Constants::bValueWindowLightTheme));
        setPalette(textPalette);
    }
    else if(SimUtils.GetTheme() == DARKTHEME)
    {
        QPalette textPalette = palette();
        textPalette.setColor(QPalette::ButtonText, QColor(Constants::rValueButtonDarkTheme, Constants::gValueButtonDarkTheme,
                                                  Constants::bValueButtonDarkTheme));
        textPalette.setColor(QPalette::WindowText, QColor(Constants::rValueWindowDarkTheme, Constants::gValueWindowDarkTheme,
                                                  Constants::bValueWindowDarkTheme));
        setPalette(textPalette);
    }
}
void SimulatorSoftware::UnhideText()
{
    QPalette textPalette = palette();
    textPalette.setColor(QPalette::ButtonText, QColor(Constants::rValueWindowText, Constants::gValueWindowText,
                                              Constants::bValueWindowText));
    textPalette.setColor(QPalette::WindowText, QColor(Constants::rValueButtonText, Constants::gValueButtonText,
                                              Constants::bValueButtonText));
    setPalette(textPalette);
}

void SimulatorSoftware::SetColorOfThePowerButton()
{
    if(SimUtils.GetPowerState() == ON && SimUtils.GetTheme() == LIGHTTHEME)
    {
        ui->PowerButton->setStyleSheet(QString("background-color: rgb(168, 180, 224)"));
    }
    else if(SimUtils.GetPowerState() == ON && SimUtils.GetTheme() == DARKTHEME)
    {
        ui->PowerButton->setStyleSheet(QString("background-color: rgb(132, 143, 181)"));
    }
    else if(SimUtils.GetPowerState() == OFF && SimUtils.GetTheme() == LIGHTTHEME)
    {
        ui->PowerButton->setStyleSheet(QString("background-color: rgb(214, 66, 66)"));
    }
    else if(SimUtils.GetPowerState() == OFF && SimUtils.GetTheme() == DARKTHEME)
    {
        ui->PowerButton->setStyleSheet(QString("background-color: rgb(189, 58, 58)"));
    }
}

void SimulatorSoftware::SetPowerButtonText()
{
    if(SimUtils.GetPowerState() == ON){ui->PowerButton->setText("Turn OFF");}
    else if(SimUtils.GetPowerState() == OFF){ui->PowerButton->setText("Turn ON");}
}

























