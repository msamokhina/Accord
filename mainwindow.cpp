#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    volumeSliderStatus(false),
    playButtonStatus(true)
{
    audioListModel = new QStringListModel(this);
    playlistModel = new QStringListModel(this);
    ui->setupUi(this);

    setMinimumWidth(562);
    setMaximumWidth(562);
    setMinimumHeight(400);
    setMaximumHeight(400);
    playButton = new QPushButton(ui->groupBox);
    playButton->setStyleSheet("QPushButton {"
                                "background-color: transparent;"
                                "border-image: url(:images/play.png);"
                                "background: none;"
                                "border: none;"
                                "background-repeat: none;"
                                "height: 50px;"
                                "width: 50px;"
                               "}");
    playButton->move(50, 0);
    playButton->show();

    QObject::connect(playButton, SIGNAL(clicked()),
                     this, SIGNAL(play()));

    QObject::connect(playButton, SIGNAL(clicked()),
            this, SLOT(setPlayPause()));

    QObject::connect(ui->nextButton, SIGNAL(clicked()),
                     this, SIGNAL(next()));

    QObject::connect(ui->prevButton, SIGNAL(clicked()),
                     this, SIGNAL(prev()));

    QObject::connect(ui->settingsButton, SIGNAL(clicked()),
                     this, SIGNAL(settings()));

    QObject::connect(ui->prevButton, SIGNAL(clicked()),
                     this, SLOT(setPrevRow()));

    QObject::connect(ui->nextButton, SIGNAL(clicked()),
                     this, SLOT(setNextRow()));

    QObject::connect(ui->curAudioListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(itemClicked(QListWidgetItem*)));

    QObject::connect(ui->curAudioListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this, SLOT(itemDoubleClicked(QListWidgetItem*)));

    QObject::connect(ui->volumeButton, SIGNAL(clicked()),
            this, SLOT(setVolumeSlider()));

    QObject::connect(ui->plusButton, SIGNAL(clicked()),
                     this, SLOT(addButtonPushed()));

}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::setAudioListModel(QStringList tracks) {
    audioListModel->setStringList(tracks);
    ui->curAudioListWidget->addItems(tracks);
}

void MainWindow::addButtonPushed() {
    emit addAudioFromDisk(this);
}

void MainWindow::itemClicked(QListWidgetItem *item){
    int position = item->listWidget()->currentRow();
    emit audioSelected(position);
}

void MainWindow::itemDoubleClicked(QListWidgetItem* item){
    int position = item->listWidget()->currentRow();
    emit audioSwitched(position);
}

void MainWindow::setPrevRow(){
    int curRow = ui->curAudioListWidget->currentRow();
    if (curRow > 0){
        --curRow;
    }
    ui->curAudioListWidget->setCurrentRow(curRow);
}

void MainWindow::setNextRow(){
    int curRow = ui->curAudioListWidget->currentRow();
    if (curRow < audioListModel->rowCount() - 1){
        ++curRow;
    }
    ui->curAudioListWidget->setCurrentRow(curRow);
}

void MainWindow::itemIndexChanged(int newRow){
    ui->curAudioListWidget->setCurrentRow(newRow, QItemSelectionModel::Current);
}

void MainWindow::sliderDurationChanged(qint64 duration){
    ui->audioTimeSlider->setSliderPosition(static_cast<int>(duration));
}

void MainWindow::setVolumeSlider() {
    if(!volumeSliderStatus){
        volumeSlider = new QSlider(Qt::Vertical, ui->centralWidget);
        volumeSlider->setRange(0,100);
        volumeSlider->show();
        volumeSliderStatus = true;
        volumeSlider->move(495,220);
    }
    else {
        delete volumeSlider;
        volumeSliderStatus = false;
    }
}

void MainWindow::setPlayPause() {
    if(playButtonStatus){
        delete playButton;
        pauseButton = new QPushButton(ui->groupBox);
        pauseButton->setStyleSheet("QPushButton {"
                                    "background-color: transparent;"
                                    "border-image: url(:images/pause.png);"
                                    "background: none;"
                                    "border: none;"
                                    "background-repeat: none;"
                                    "height: 50px;"
                                    "width: 50px;"
                                   "}");
        playButton->move(50, 0);
        pauseButton->show();
        playButtonStatus = false;

        QObject::connect(pauseButton, SIGNAL(clicked()),
                this, SIGNAL(pause()));

        QObject::connect(pauseButton, SIGNAL(clicked()),
                this, SLOT(setPlayPause()));

    }
    else {
        delete pauseButton;
        playButton = new QPushButton(ui->groupBox);
        playButton->setStyleSheet("QPushButton {"
                                    "background-color: transparent;"
                                    "border-image: url(:images/play.png);"
                                    "background: none;"
                                    "border: none;"
                                    "background-repeat: none;"
                                    "height: 50px;"
                                    "width: 50px;"
                                   "}");
        playButton->move(50, 0);
        playButton->show();
        playButtonStatus = true;

        QObject::connect(playButton, SIGNAL(clicked()),
                this, SIGNAL(play()));

        QObject::connect(playButton, SIGNAL(clicked()),
                this, SLOT(setPlayPause()));
    }
}
