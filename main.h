#ifndef MAIN_H
#define MAIN_H

#include <QApplication>
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QLineEdit>
#include <QMenuBar>

class ClipTrack : public QWidget {
    Q_OBJECT

public:
    ClipTrack(QWidget* parent = nullptr);

private slots:
    void track_clipboard();
    void pause_clipboard();
    void resume_clipboard();
    void clear_cliptrack();
	void copy_cliptrack_to_clipboard();
    void save_clipboard();
    void load_clipboard();
    void clear_clipboard();
	void toggle_textbox_read_only(bool checked);
    void show_about();

private:
	QMenuBar* menuBar;
    QLabel* active_clipboard;
    QTextEdit* textbox;
    QPushButton* pause_button;
    QPushButton* resume_button;
    QPushButton* clear_field_button;

    QClipboard* clipboard;
    QString last_clipboard_text;
    bool clipboard_paused;
};

#endif // MAIN_H