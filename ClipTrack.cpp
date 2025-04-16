#include "main.h"

/*
* Hey. This is my first Qt (and C++) project [apart from "Hello World"].
* I made this program to learn Qt and C++, so don't expect it to be perfect.
*/

ClipTrack::ClipTrack(QWidget* parent) : QWidget(parent), clipboard_paused(false) {
    clipboard = QApplication::clipboard(); // Initialize clipboard before use

    setWindowTitle("ClipTrack");
    setGeometry(100, 100, 400, 400);

	// Menu bar
	menuBar = new QMenuBar(this);


    // File menu
	QMenu* fileMenu = menuBar->addMenu("File");
	QAction* saveAction = fileMenu->addAction("Save ClipTrack as .txt");
	QAction* loadAction = fileMenu->addAction("Load ClipTrack from .txt");
	fileMenu->addSeparator();
	QAction* copyAction = fileMenu->addAction("Copy ClipTrack to Clipboard");
	fileMenu->addSeparator();
	QAction* clearAction = fileMenu->addAction("Clear ClipTrack");

	// Features menu
    QMenu* specialMenu = menuBar->addMenu("Features");
	QAction* clearClipboardAction = specialMenu->addAction("Clear Clipboard");
	QAction* toggleReadOnly = specialMenu->addAction("Toggle read-only Textbox");
	toggleReadOnly->setCheckable(true);

	// Help menu
	QMenu* helpMenu = menuBar->addMenu("Help");
	QAction* aboutAction = helpMenu->addAction("About ClipTrack");

	// All menu bar actions
	connect(saveAction, &QAction::triggered, this, &ClipTrack::save_clipboard);
	connect(loadAction, &QAction::triggered, this, &ClipTrack::load_clipboard);
	connect(copyAction, &QAction::triggered, this, &ClipTrack::copy_cliptrack_to_clipboard);
	connect(clearAction, &QAction::triggered, this, &ClipTrack::clear_cliptrack);

	connect(clearClipboardAction, &QAction::triggered, this, &ClipTrack::clear_clipboard);
	connect(toggleReadOnly, &QAction::toggled, this, &ClipTrack::toggle_textbox_read_only);

	connect(aboutAction, &QAction::triggered, this, &ClipTrack::show_about);


	// Set the layout
    QVBoxLayout* layout = new QVBoxLayout(this);

	// Add the menu bar to the layout
    layout->setMenuBar(menuBar);

	// Label for telling the user that ClipTrack is active
    active_clipboard = new QLabel("ClipTrack is active", this);
    layout->addWidget(active_clipboard);

	// The textbox where the clipboard text will be displayed
    textbox = new QTextEdit(this);
    layout->addWidget(textbox);


    pause_button = new QPushButton("Pause ClipTrack", this);
    connect(pause_button, &QPushButton::clicked, this, &ClipTrack::pause_clipboard);
    layout->addWidget(pause_button);

    resume_button = new QPushButton("Resume ClipTrack", this);
    resume_button->setEnabled(false);
    connect(resume_button, &QPushButton::clicked, this, &ClipTrack::resume_clipboard);
    layout->addWidget(resume_button);

    clear_field_button = new QPushButton("Clear ClipTrack", this);
    connect(clear_field_button, &QPushButton::clicked, this, &ClipTrack::clear_cliptrack);
    layout->addWidget(clear_field_button);

    connect(clipboard, &QClipboard::dataChanged, this, &ClipTrack::track_clipboard);

	// RIP to my former self, who thought this was a good idea - Timer and connect at the same time lol
    /*
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ClipTrack::track_clipboard);
    timer->start(1000);
    */
    }

void ClipTrack::track_clipboard() {
    if (!clipboard_paused) {
        QString clipboard_text = clipboard->text();
        if (clipboard_text != last_clipboard_text) {
			// Get the current text in the textbox
			QString current_text = textbox->toPlainText();

            // Check if the clipboard is holding the same text as the cliptrack -> User wants to copy CT
			if (clipboard_text == current_text) {
				return;
			}
            
			// Set the new clipboard text to the last_clipboard_text for comparison
            last_clipboard_text = clipboard_text;

			// Check if the textbox is empty, because we dont want the "\n" in first line
			if (current_text.isEmpty()) {
				// Set the textbox to the new clipboard text
				textbox->setPlainText(clipboard_text);
			}
			else {
				// Append the new clipboard text to the existing text
                textbox->setPlainText(current_text + "\n" + clipboard_text);
        }
    }
	}
}

void ClipTrack::pause_clipboard() {
    clipboard_paused = true;
    active_clipboard->setText("ClipTrack is paused");
    pause_button->setEnabled(false);
    resume_button->setEnabled(true);
}

void ClipTrack::resume_clipboard() {
    clipboard_paused = false;
    active_clipboard->setText("ClipTrack is active");
    resume_button->setEnabled(false);
    pause_button->setEnabled(true);
}

void ClipTrack::clear_cliptrack() {
    textbox->setPlainText("");
}

void ClipTrack::copy_cliptrack_to_clipboard() {
	QString text = textbox->toPlainText();
	if (!text.isEmpty()) {
		clipboard->setText(text);
	}
}

void ClipTrack::save_clipboard() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Clipboard", "", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << textbox->toPlainText();
            file.close();
        }
    }
}

void ClipTrack::load_clipboard() {
    QString fileName = QFileDialog::getOpenFileName(this, "Load Clipboard", "", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString file_content = in.readAll();
            file.close();

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Load Clipboard", "Do you want to load the clipboard from the file (yes), or import it in the textbox (no)?",
                QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                clipboard->setText(file_content);
            }
            else {
                textbox->setPlainText(file_content);
            }
        }
    }
}

void ClipTrack::clear_clipboard() {
    clipboard->clear();
}

void ClipTrack::toggle_textbox_read_only(bool checked) {
	if (checked) {
		textbox->setReadOnly(true);
	}
	else {
		textbox->setReadOnly(false);
	}
}

void ClipTrack::show_about() {
    QMessageBox aboutBox(this);
    aboutBox.setWindowTitle("About ClipTrack");

    // HTML-Inhalt mit anklickbaren Links
    aboutBox.setTextFormat(Qt::RichText); // Aktiviert HTML-Rendering
    aboutBox.setText(
        "<p>ClipTrack is a simple clipboard tracker which allows you to copy multiple things and have them in one Textbox.</p>"
        "<p>My GitHub: <a href='https://github.com/inviHD'>https://github.com/inviHD</a></p>"
        "<p>Credits:</p>"
        "<ul>"
        "<li>Icon: <a href='https://icon-icons.com/icon/interface-clipboard-file-paper-document-office/132996'>Interface clipboard file paper Icon</a> by Royyan Wijaya, licensed under CC BY 4.0.</li>"
        "<li>Qt Framework: Licensed under LGPL.</li>"
        "</ul>"
        "<p>For more information visit:</p>"
        "<ul>"
        "<li><a href='https://creativecommons.org/licenses/by/4.0/'>CC BY 4.0</a></li>"
        "<li><a href='https://www.qt.io/licensing/open-source-lgpl-obligations'>Qt LGPL Obligations</a></li>"
        "<li><a href='https://www.gnu.org/licenses/lgpl-3.0.html'>LGPL v3.0</a></li>"
        "</ul>"
        "<p>Thanks for using this Program :D<br>~ Invi</p>"
    );

    aboutBox.setStandardButtons(QMessageBox::Ok);
    aboutBox.exec();
}