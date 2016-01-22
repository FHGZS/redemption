/*
 * 
 */

#ifndef FORM_QT_HPP
#define FORM_QT_HPP


#include "transport/socket_transport.hpp"
#include "rdp/rdp.hpp"
#include "../src/front/front_Qt.hpp"

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtCore/QSocketNotifier>
#include <QtGui/QLineEdit>
#include <QtGui/QFormLayout>
#include <QtGui/QDialog>
#include <QtGui/QPushButton>




class DialogOptions_Qt : public QDialog
{
    
Q_OBJECT

public:
    Front_Qt_API       * _front;
    const int            _width;
    const int            _height;
    QFormLayout          _layout;
    QLabel               _title;
    QPushButton          _buttonSave;
    QPushButton          _buttonCancel;
    
    
    DialogOptions_Qt(Front_Qt_API * front, QWidget * parent) 
        : QDialog(parent)
        , _front(front)
        , _width(400)
        , _height(300)
        , _layout(this)
        , _title("Options", this)
        , _buttonSave("Save", this)
        , _buttonCancel("Cancel", this)
    {
        this->setWindowTitle("Options");
        this->setAttribute(Qt::WA_DeleteOnClose);
        this->setFixedSize(this->_width, this->_height);
        
        this->_layout.addRow(&(this->_title));
        this->setLayout(&(this->_layout));
        
        QRect rectSave(QPoint(160, 266), QSize(110, 24)); 
        this->_buttonSave.setToolTip(this->_buttonSave.text());
        this->_buttonSave.setGeometry(rectSave);
        this->_buttonSave.setCursor(Qt::PointingHandCursor);
        this->QObject::connect(&(this->_buttonSave)   , SIGNAL (pressed()),  this, SLOT (savePressed()));
        this->QObject::connect(&(this->_buttonSave)   , SIGNAL (released()), this, SLOT (saveReleased()));
        this->_buttonSave.setFocusPolicy(Qt::StrongFocus);
        
        QRect rectCancel(QPoint(280, 266), QSize(110, 24)); 
        this->_buttonCancel.setToolTip(this->_buttonCancel.text());
        this->_buttonCancel.setGeometry(rectCancel);
        this->_buttonCancel.setCursor(Qt::PointingHandCursor);
        this->QObject::connect(&(this->_buttonCancel)   , SIGNAL (pressed()),  this, SLOT (cancelPressed()));
        this->QObject::connect(&(this->_buttonCancel)   , SIGNAL (released()), this, SLOT (cancelReleased()));
        this->_buttonCancel.setFocusPolicy(Qt::StrongFocus);
        
        QDesktopWidget* desktop = QApplication::desktop();
        int centerW = (desktop->width()/2)  - (this->_width/2);
        int centerH = (desktop->height()/2) - (this->_height/2);
        this->move(centerW, centerH);
        
        this->show();
    }
    
    ~DialogOptions_Qt() {}

    
public Q_SLOTS:
    void savePressed() {}
    
    void saveReleased() {
        this->_front->updateForm(true);
        this->close();
    }
    
    void cancelPressed() {}
    
    void cancelReleased() {
        this->_front->updateForm(true);
        this->close();
    }
    
};



class Form_Qt : public QWidget
{
    
Q_OBJECT    
    
public:
    Front_Qt_API       * _front;
    const int            _width;
    const int            _height;
    QFormLayout          _formLayout;
    QLineEdit            _userNameField;
    QLineEdit            _IPField; 
    QLineEdit            _PWDField;
    QLineEdit            _portField;
    QLabel               _errorLabel;
    QLabel               _userNameLabel;           
    QLabel               _IPLabel;  
    QLabel               _PWDLabel;  
    QLabel               _portLabel;
    QPushButton          _buttonConnexion;
    QPushButton          _buttonOptions;
    
    
    Form_Qt(Front_Qt_API * front)
        : QWidget()
        , _front(front)
        , _width(400)
        , _height(300)
        , _formLayout(this)
        , _userNameField("", this)
        , _IPField("", this)
        , _PWDField("", this)
        , _portField("", this)
        , _errorLabel(   QString(""), this)
        , _userNameLabel(QString("User name : "), this)         
        , _IPLabel(      QString("IP serveur :"), this)   
        , _PWDLabel(     QString("Password :  "), this)   
        , _portLabel(    QString("Port :      "), this) 
        , _buttonConnexion("Connexion", this)
        , _buttonOptions("Options", this)
    {
        this->setWindowTitle("Desktop Linker");
        this->setAttribute(Qt::WA_DeleteOnClose);
        this->setFixedSize(this->_width, this->_height);
        
        this->_PWDField.setEchoMode(QLineEdit::Password);
        this->_PWDField.setInputMethodHints(Qt::ImhHiddenText | Qt::ImhNoPredictiveText | Qt::ImhNoAutoUppercase);
        this->_formLayout.addRow(&(this->_IPLabel)      , &(this->_IPField));
        this->_formLayout.addRow(&(this->_userNameLabel), &(this->_userNameField));
        this->_formLayout.addRow(&(this->_PWDLabel)     , &(this->_PWDField));
        this->_formLayout.addRow(&(this->_portLabel)    , &(this->_portField));
        this->_formLayout.addRow(&(this->_errorLabel));
        this->setLayout(&(this->_formLayout));
        
        QRect rectConnexion(QPoint(280, 256), QSize(110, 24)); 
        this->_buttonConnexion.setToolTip(this->_buttonConnexion.text());
        this->_buttonConnexion.setGeometry(rectConnexion);
        this->_buttonConnexion.setCursor(Qt::PointingHandCursor);
        this->QObject::connect(&(this->_buttonConnexion)   , SIGNAL (pressed()),  this, SLOT (connexionPressed()));
        this->QObject::connect(&(this->_buttonConnexion)   , SIGNAL (released()), this, SLOT (connexionReleased()));
        this->_buttonConnexion.setFocusPolicy(Qt::StrongFocus);
        
        QRect rectOptions(QPoint(10, 256), QSize(110, 24)); 
        this->_buttonOptions.setToolTip(this->_buttonOptions.text());
        this->_buttonOptions.setGeometry(rectOptions);
        this->_buttonOptions.setCursor(Qt::PointingHandCursor);
        this->QObject::connect(&(this->_buttonOptions)   , SIGNAL (pressed()),  this, SLOT (optionsPressed()));
        this->QObject::connect(&(this->_buttonOptions)   , SIGNAL (released()), this, SLOT (optionsReleased()));
        this->_buttonOptions.setFocusPolicy(Qt::StrongFocus);
        
        QDesktopWidget* desktop = QApplication::desktop();
        int centerW = (desktop->width()/2)  - (this->_width/2);
        int centerH = (desktop->height()/2) - (this->_height/2);
        this->move(centerW, centerH);
    }
    
    ~Form_Qt() {
        this->_front->closeFromForm();
    }
    
    void set_ErrorMsg(std::string txt) {
        this->_errorLabel.clear();
        this->_errorLabel.setText(QString(txt.c_str()));
    }
    
    void set_IPField(std::string str) {
        this->_IPField.clear();
        this->_IPField.insert(QString(str.c_str()));
    }
    
    void set_userNameField(std::string str) {
        this->_userNameField.clear();
        this->_userNameField.insert(QString(str.c_str()));
    }

    void set_PWDField(std::string str) {
        this->_PWDField.clear();
        this->_PWDField.insert(QString(str.c_str()));
    }
       
    void set_portField(int str) {
        this->_portField.clear();
        if (str == 0) {
            this->_portField.insert(QString(""));
        } else {
            this->_portField.insert(QString(std::to_string(str).c_str()));
        }
    }
    
    std::string get_IPField() {
        return this->_IPField.text().toStdString();
    }
    
    std::string get_userNameField() {
        return this->_userNameField.text().toStdString();
    }
    
    std::string get_PWDField() {
        return this->_PWDField.text().toStdString();
    }
    
    int get_portField() {
        return this->_portField.text().toInt();
    }
    
    void setEnable(bool enable) {
        this->_buttonConnexion.setEnabled(enable);
        this->_buttonOptions.setEnabled(enable);
    }
    
    
private Q_SLOTS:
    void connexionPressed() {
        this->_front->connexionPressed();
    }
    
    void connexionReleased() {
        this->_front->connexionReleased();
    }
    
    void optionsPressed() {}
    
    void optionsReleased() {
        this->setEnable(false);
        this->update();
        DialogOptions_Qt * dia = new DialogOptions_Qt(this->_front, this);
    }
};



class Screen_Qt : public QWidget
{
    
Q_OBJECT
    
public:
    Front_Qt_API       * _front;
    QPushButton          _buttonCtrlAltDel;
    QPushButton          _buttonRefresh;
    QPushButton          _buttonDisconnexion;
    QColor               _penColor;
    QPixmap              _cache;
    QPainter             _cache_painter;
    
    
    Screen_Qt (Front_Qt_API * front)
    : QWidget()
    , _front(front)
    , _buttonCtrlAltDel("CTRL + ALT + DELETE", this)
    , _buttonRefresh("Refresh", this)
    , _buttonDisconnexion("Disconnexion", this)
    , _penColor(Qt::black)
    , _cache(this->_front->_info.width, this->_front->_info.height)
    , _cache_painter(&(this->_cache))
    {
        this->_cache_painter.fillRect(0, 0, this->_front->_info.width, this->_front->_info.height, QColor(0, 0, 0, 0));
        this->setUpdate();
        this->setMouseTracking(true);
        this->installEventFilter(this);
        this->setAttribute(Qt::WA_NoSystemBackground, true);
        this->setAttribute(Qt::WA_DeleteOnClose);
    
        QRect rectCtrlAltDel(QPoint(0, this->_front->_info.height+1),QSize(this->_front->_info.width/3, 20));
        this->_buttonCtrlAltDel.setToolTip(this->_buttonCtrlAltDel.text());
        this->_buttonCtrlAltDel.setGeometry(rectCtrlAltDel);
        this->_buttonCtrlAltDel.setCursor(Qt::PointingHandCursor);
        this->QObject::connect(&(this->_buttonCtrlAltDel)  , SIGNAL (pressed()),  this, SLOT (CtrlAltDelPressed()));
        this->QObject::connect(&(this->_buttonCtrlAltDel)  , SIGNAL (released()), this, SLOT (CtrlAltDelReleased()));
        this->_buttonCtrlAltDel.setFocusPolicy(Qt::NoFocus);

        QRect rectRefresh(QPoint(this->_front->_info.width/3, this->_front->_info.height+1),QSize(this->_front->_info.width/3, 20));
        this->_buttonRefresh.setToolTip(this->_buttonRefresh.text());
        this->_buttonRefresh.setGeometry(rectRefresh);
        this->_buttonRefresh.setCursor(Qt::PointingHandCursor);
        this->QObject::connect(&(this->_buttonRefresh)     , SIGNAL (pressed()),  this, SLOT (RefreshPressed()));
        this->QObject::connect(&(this->_buttonRefresh)     , SIGNAL (released()), this, SLOT (RefreshReleased()));
        this->_buttonRefresh.setFocusPolicy(Qt::NoFocus);
        
        QRect rectDisconnexion(QPoint(((this->_front->_info.width/3)*2), this->_front->_info.height+1),QSize(this->_front->_info.width-((this->_front->_info.width/3)*2), 20));
        this->_buttonDisconnexion.setToolTip(this->_buttonDisconnexion.text());
        this->_buttonDisconnexion.setGeometry(rectDisconnexion);
        this->_buttonDisconnexion.setCursor(Qt::PointingHandCursor);
        this->QObject::connect(&(this->_buttonDisconnexion), SIGNAL (pressed()),  this, SLOT (disconnexionPressed()));
        this->QObject::connect(&(this->_buttonDisconnexion), SIGNAL (released()), this, SLOT (disconnexionRelease()));
        this->_buttonDisconnexion.setFocusPolicy(Qt::NoFocus);
        
        this->setFocusPolicy(Qt::StrongFocus);
    }
    
    ~Screen_Qt() {
        this->_front->closeFromScreen();
    }
    
    QPainter & paintCache() {
        return this->_cache_painter;
    }
    
    void paintEvent(QPaintEvent * event) {
        Q_UNUSED(event);
        
        QPen                 pen; 
        QPainter             painter;
        painter.begin(this);
        painter.setRenderHint(QPainter::Antialiasing);
        pen.setWidth(1);
        pen.setBrush(this->_penColor);
        painter.setPen(pen);
        const QPixmap pxmp(this->_cache);
        painter.drawPixmap(0, 0, pxmp);
        painter.end();
    }
    
    QPixmap * getCache() {
        return &(this->_cache);
    }
    
    void setUpdate() {
        std::string title = "Desktop from [" + this->_front->_targetIP +  "].";
        this->setWindowTitle(QString(title.c_str()));
        this->setFixedSize(this->_front->_info.width, this->_front->_info.height+20);   
        QDesktopWidget* desktop = QApplication::desktop();
        int centerW = (desktop->width()/2)  - (this->_front->_info.width/2);
        int centerH = (desktop->height()/2) - ((this->_front->_info.height+20)/2);
        this->move(centerW, centerH);
    }
    
    void setPenColor(QColor color) {
        this->_penColor = color; 
    }
    
    
private:
    void mousePressEvent(QMouseEvent *e) {
        this->_front->mousePressEvent(e);
    }
    
    void mouseReleaseEvent(QMouseEvent *e) {
        this->_front->mouseReleaseEvent(e);
    }
    
    void keyPressEvent(QKeyEvent *e) { 
        this->_front->keyPressEvent(e);
    }
    
    void keyReleaseEvent(QKeyEvent *e) {
        this->_front->keyReleaseEvent(e);
    }
    
    void wheelEvent(QWheelEvent *e) {
        this->_front->wheelEvent(e);
    }
    
    bool eventFilter(QObject *obj, QEvent *e) {
        this->_front->eventFilter(obj, e);
        return false;
    }
    
    
private Q_SLOTS:
    void RefreshPressed() {
        this->_front->RefreshPressed();
    }
    
    void RefreshReleased() {
        this->_front->RefreshReleased();
    }
    
    void CtrlAltDelPressed() {
        this->_front->CtrlAltDelPressed();    
    }
    
    void CtrlAltDelReleased() {
        this->_front->CtrlAltDelReleased();
    }
    
    void disconnexionPressed() {
        this->_front->disconnexionPressed();
    }
    
    void disconnexionRelease(){
        this->_front->disconnexionReleased();
    }
};



class Connector_Qt : public QObject
{
    
Q_OBJECT
    
public:
    Front_Qt_API    * _front;
    QSocketNotifier * _sckRead;
    mod_api         * _callback;
    SocketTransport * _sck;
    int               _client_sck;
    
    
    Connector_Qt(Front_Qt_API * front, QWidget * parent) 
    : QObject(parent)
    , _front(front)
    , _sckRead(nullptr)
    , _callback(nullptr)
    , _sck(nullptr)
    , _client_sck(0)
    {}
    
    ~Connector_Qt() {
        this->drop_connexion();
    }
    
    void drop_connexion() {
        if (this->_callback != nullptr) {
            this->_callback->send_disconnect_ultimatum();
            delete (this->_callback);
            this->_callback = nullptr;
            this->_front->_callback = nullptr;
        }
        if (this->_sckRead != nullptr) {
            delete (this->_sckRead);
            this->_sckRead = nullptr;
        }
        if (this->_sck != nullptr) {
            delete (this->_sck);
            this->_sck = nullptr;
            std::cout << "Disconnected from [" << this->_front->_targetIP << "]." << std::endl;
        }
    }
    
    bool connect() {
        const char * name(this->_front->_userName.c_str());      
        const char * targetIP(this->_front->_targetIP.c_str());                 
        std::string errorMsg("Cannot connect to [" + this->_front->_targetIP +  "].");
        
        //std::cout << name << " " << this->_front->_pwd << " " << this->_front->_targetIP.c_str() << " " << this->_front->_port << std::endl;

        this->_client_sck = ip_connect(targetIP, this->_front->_port, this->_front->_nbTry, this->_front->_retryDelay, this->_front->verbose);

        if (this->_client_sck > 0) {
            try {
                std::string error_message;
                this->_sck = new SocketTransport( name
                                                , this->_client_sck
                                                , targetIP
                                                , this->_front->_port
                                                , this->_front->verbose
                                                , &error_message
                                                );
                std::cout << "Connected to [" << targetIP <<  "]." << std::endl;
                return true;
                
            } catch (const std::exception & e) {
                std::cout << errorMsg << std::endl;
                std::string windowErrorMsg("<font color='Red'>"+errorMsg+"</font>");
                this->_front->disconnect(windowErrorMsg);
                return false;
            }
        } else {
            std::cout << errorMsg << std::endl;
            std::string windowErrorMsg("<font color='Red'>"+errorMsg+"</font>");
            this->_front->disconnect(windowErrorMsg);
            return false;
        }
    }
    
    void listen() {
        const char * name(this->_front->_userName.c_str());      
        const char * pwd(this->_front->_pwd.c_str()); 
        const char * targetIP(this->_front->_targetIP.c_str());         
        const char * localIP(this->_front->_localIP.c_str());
        std::string errorMsg("Connected to [" + this->_front->_targetIP +  "].");
        
        Inifile ini;
        ModRDPParams mod_rdp_params( name
                                    , pwd
                                    , targetIP
                                    , localIP
                                    , 2
                                    , 0
                                    );  
        mod_rdp_params.device_id                       = "device_id";
        mod_rdp_params.enable_tls                      = false;
        mod_rdp_params.enable_nla                      = false;
        //mod_rdp_params.enable_krb                      = false;
        //mod_rdp_params.enable_clipboard                = true;
        mod_rdp_params.enable_fastpath                 = false; 
        mod_rdp_params.enable_mem3blt                  = false;
        mod_rdp_params.enable_bitmap_update            = true;
        mod_rdp_params.enable_new_pointer              = false;
        //mod_rdp_params.rdp_compression                 = 0;
        //mod_rdp_params.error_message                   = nullptr;
        //mod_rdp_params.disconnect_on_logon_user_change = false;
        //mod_rdp_params.open_session_timeout            = 0;
        //mod_rdp_params.certificate_change_action       = 0;
        //mod_rdp_params.extra_orders                    = "";
        mod_rdp_params.server_redirection_support        = true;
        
        LCGRandom gen(0); // To always get the same client random, in tests

        try {
            this->_callback = new mod_rdp(*(this->_sck), *(this->_front), this->_front->_info, ini.get_ref<cfg::mod_rdp::redir_info>(), gen, mod_rdp_params);
        } catch (const std::exception & e) {
            std::cout << errorMsg << std::endl;
                std::string labelErrorMsg("<font color='Red'>"+errorMsg+"</font>");
                this->_front->disconnect(labelErrorMsg);
        }
        
        if (this->_callback != nullptr) {
            this->_front->_callback = this->_callback;
            this->_sckRead = new QSocketNotifier(this->_client_sck, QSocketNotifier::Read, this);
            this->QObject::connect(this->_sckRead, SIGNAL(activated(int)), this, SLOT(call_Draw()));
        } else {
            std::cout << errorMsg << std::endl;
            std::string labelErrorMsg("<font color='Red'>"+errorMsg+"</font>");
            this->_front->disconnect(labelErrorMsg);
        }
    }
    
public Q_SLOTS:
    void call_Draw() {
        this->_front->call_Draw();
    }
    
};

#endif