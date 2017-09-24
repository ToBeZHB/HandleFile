#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

	//branch test 3322233
    ui->setupUi(this);
    setWindowTitle("文件批处理");
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit_2->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString s = QFileDialog::getOpenFileName(this,"选择TXT文件","./","*.txt");
    if(s.isEmpty()){
        return;
    }
    ui->lineEdit->setText(s);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString s = QFileDialog::getExistingDirectory(this,"选择保存文件夹","./");
    if(s.isEmpty()){
        return;
    }
    ui->lineEdit_2->setText(s);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString txt_path = ui->lineEdit->text();
    QString save_path = ui->lineEdit_2->text();
    if(txt_path.isEmpty()||save_path.isEmpty()){
        ui->statusBar->showMessage("请选择路径");
        return;
    }
    QFile file(txt_path);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        ui->statusBar->showMessage("文件打开失败！");
        return;
    }

    QTextStream ts(&file);
    int i=0;
    QString dirname = "";
    QFile *cf;
    QTextStream *txtOutput;
    while(!ts.atEnd()){
        if(i == 0){
            dirname = ts.readLine();
            QDir d(save_path+"/"+dirname);
            if(!d.exists()){
                qDebug()<<"create Dir"<<save_path+"\/"+dirname;
                d.mkdir(save_path+"\/"+dirname);
            }
            cf = new QFile(save_path+"\/"+dirname+"\/file.txt");
            txtOutput = new QTextStream(cf);
            if(!cf->open(QIODevice::WriteOnly | QIODevice::Text))
            {
                ui->statusBar->showMessage("写入文件失败");
                return;
            }
            i++;
        }else{
            QString line = ts.readLine();
            *txtOutput << line<<endl;
            if(line.trimmed().length()==0){
                //保存文件
                cf->close();
                i=0;
                delete cf;
                delete txtOutput;
            }
        }
    }
    ui->statusBar->showMessage("文件处理完成！");

}
