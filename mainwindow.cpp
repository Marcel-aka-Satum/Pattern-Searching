#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QLineEdit>
#include <QString>
#include "PatternSearching.h"
#include "qmessagebox.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Pattern Searching");
}

MainWindow::~MainWindow()
{
    delete ui;
}



/*
    This function will use the functionality of pattern serach upon button click "search".
    After clicking button software should highlight text it was looking for in the "look" for table
    + it should return positions of found searches.
    ex. "text" : "Hello world, world hello"
        "look for": "world"
        Positions of found searches should return: (6, 0), (13, 0)
        because the "look for" starts at index 6 and 13 and is on the first line of the file
*/
void MainWindow::on_pushButton_clicked()
{
    /*
    Get first text from Look for
    Get text from "text" label
    Then check if Union needs to be done
    Perform pattern search
    Highlight if "look for" was found in text
    Return positions of found searches in table "Positions of found searches"
    */
    QString look_for_2 = ui->textEdit_4->toPlainText();
    QString look_for_string = ui->textEdit_2->toPlainText();
    QString text = ui->textEdit->toPlainText();

    string textToString = text.toStdString();
    string look_for_stringToString = look_for_string.toStdString();
    string look_for_stringToString2 = look_for_2.toStdString();

    ofstream MyFile("lookFor.txt");
    MyFile << textToString;
    MyFile.close();

    ui->textEdit->setText(text);

    //user didnt pass any regexes but still wants to use search option
    if(look_for_stringToString == "" && look_for_stringToString2 == ""){
        QMessageBox::information(this, tr("error unie/doorsnede"), tr("please provide some argument to use the search function"));
        return;
    }

    // user didnt pass second regex
    if(look_for_stringToString2 == ""){
        if(ui->checkBox->isChecked()){
            QMessageBox::information(this, tr("error unie/doorsnede"), tr("you need two arguments to use Union option"));
            return;
        }
        //object used to perform patternsearching and store regexes
        PatternSearching* patternObject = new PatternSearching("lookFor.txt", look_for_stringToString);
        patternObject->search();

        string aantalPostions = to_string(patternObject->getAantalPositions());
        QString aantalPostionsQstring = QString::fromStdString(aantalPostions);

        string coordinates = patternObject->getPositions();
        QString Qcoordinates = QString::fromStdString(coordinates);
        ui->textEdit_3->setText(Qcoordinates);
        ui->textEdit_5->setText(aantalPostionsQstring);
        delete patternObject;
        highlightText(textToString);
    } else if(look_for_stringToString2 != ""){ // user passed second regex
        if(ui->checkBox->isChecked()){ // unie
            //object used to perform patternsearching and store regexes
            PatternSearching* patternObject = new PatternSearching("lookFor.txt", look_for_stringToString, look_for_stringToString2, false);
            patternObject->search();

            string aantalPostions = to_string(patternObject->getAantalPositions());
            QString aantalPostionsQstring = QString::fromStdString(aantalPostions);

            string coordinates = patternObject->getPositions();
            QString Qcoordinates = QString::fromStdString(coordinates);
            ui->textEdit_3->setText(Qcoordinates);
            ui->textEdit_5->setText(aantalPostionsQstring);
            delete patternObject;
            highlightText(textToString);
        }
    }

}


/*
Given string of input highlight all the words that user looks for in yellow color
*/
void MainWindow::highlightText(string text){
    QString look_for_string = ui->textEdit_2->toPlainText();
    string look_for_stringToString = look_for_string.toStdString();

    QString look_for_string2 = ui->textEdit_4->toPlainText();
    string lookFor2 = look_for_string2.toStdString();

    int end = 0;
    int startCount = 0;
    bool isEqual = false;
    QTextEdit* edit = ui->textEdit;
    QTextCharFormat fmt;
    fmt.setBackground(Qt::yellow);

    for(auto i : text){
        if(i == look_for_stringToString[startCount]){
            isEqual = true;
            startCount++;
        } else {
            isEqual = false;
            startCount = 0;
        }
        if(startCount == look_for_stringToString.size() && isEqual == true){
            QTextCursor cursor(edit->document());
            cursor.setPosition(end+1 - look_for_stringToString.size(), QTextCursor::MoveAnchor);
            cursor.setPosition(end+1, QTextCursor::KeepAnchor);
            cursor.setCharFormat(fmt);
        }
        end++;
    }


    //union if user passed second regex
    if(lookFor2 != ""){
        startCount = 0;
        end = 0;
        if(ui->checkBox->isChecked()){
            for(auto i : text){
                if(i == lookFor2[startCount]){
                    isEqual = true;
                    startCount++;
                } else {
                    isEqual = false;
                    startCount = 0;
                }
                if(startCount == lookFor2.size() && isEqual == true){
                    QTextCursor cursor(edit->document());
                    cursor.setPosition(end+1 - lookFor2.size(), QTextCursor::MoveAnchor);
                    cursor.setPosition(end+1, QTextCursor::KeepAnchor);
                    cursor.setCharFormat(fmt);
                }
                end++;
            }
        }
    }

}


