#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include "pencil.h"



class QToolButton;
class QScrollArea;
class PaintArea;
class PaintWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PaintWindow(QWidget *parent = 0);
    virtual ~PaintWindow();

    void createMenu();
    void createToolBar();

//    void keyPressEvent(QKeyEvent *ev);

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void onNew();
    void onOpen();
    void onSave();
    void onSaveAs();
    void onExit();
    void onZoomIn();
    void onZoomOut();
    void onRestore();
    void onClear();
    void onChoose();
    void onTranslation();
    void onRotate();
    void onZoom();
    void onDrawNone();
    void onDrawLinecenter();
    void onDrawLineBresenham();
    void onDrawRect();
    void onDrawCircle();
    void onDrawEllipse();
    void onDrawPolygon();
    void onScanFill();
    void onSeedFill();
    void onCohenSutherland();
    void onCutLineCenter();
    void onFillRectangle();
    void ShowPenWidth(int);
    void ShowPenStyle(int);

    void ShowPenColor();

    void onDrawDashLine();
    void onDrawDefaultLine();

private:
    QMenuBar *m_menuBar;
    QAction  *m_actNew;
    QAction  *m_actOpen;
    QAction  *m_actSave;
    QAction  *m_actSaveAs;
    QAction  *m_actExit;
    QAction  *m_actZoomIn;
    QAction  *m_actZoomOut;
    QAction  *m_actRestore;
    QAction  *m_actClear;
    QAction  *m_actLinecenter;
    QAction  *m_actLineBresenham;
    QAction  *m_actRect;
    QAction  *m_actCircle;
    QAction  *m_actEllipse;
    QAction  *m_actPolygon;
    QAction  *m_actScan;
    QAction  *m_actSeed;
    QAction  *m_actCohenSutherland;
    QAction  *m_actCutLineCenter;


    QToolBar    *m_toolBar;
    QToolButton *m_zoomInBtn;
    QToolButton *m_zoomOutBtn;
    QToolButton *m_restoreBtn;
    QToolButton *m_clearBtn;
    QToolButton *m_chooseBtn;
    QToolButton *m_translationBtn;
    QToolButton *m_rotateBtn;
    QToolButton *m_zoomBtn;

    QLabel *penWidthLabel;
    QSpinBox *penWidthSpinBox;
    QLabel *penStyleLabel;
    QComboBox *penStyleComboBox;

    QToolButton *penColorBtn;
    QFrame *penColorFrame;

    QScrollArea *m_scrollArea;
    PaintArea   *m_paintArea;


    QString      m_curFile;

    Pencil* pencil;
};

#endif // PAINTWINDOW_H
