#include <QtWidgets>
#include "paintwindow.h"
#include "paintarea.h"



PaintWindow::PaintWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setMinimumSize(1500, 750);

    m_scrollArea = new QScrollArea();
    m_paintArea = new PaintArea(0,pencil);
    m_scrollArea->setWidget(m_paintArea);
    m_scrollArea->widget()->setMinimumSize(1500, 750);
    this->setCentralWidget(m_scrollArea);

    createMenu();//上面的菜单栏
    createToolBar();//下面的工具栏

    m_curFile = "untitled.png";

    pencil= new Pencil();

    m_paintArea->setFocus();  //改变焦点使paintarea可以监听

}

//void PaintWindow::keyPressEvent(QKeyEvent *ev)
//{
//    qDebug() << "键盘" <<endl;
//    m_paintArea->m_tmpImage = m_paintArea->m_tmpImageTrans;
//    if(ev->key() == Qt::Key_W)
//    {
//        if(m_paintArea->m_drawShapeType == PaintArea::EnumRotateShape)
//        {
//            qDebug() << "按了旋转了" <<endl;
//            m_paintArea->thita = 1;
//            m_paintArea->paintImage(m_paintArea->m_tmpImage);

//        }
//        else if(m_paintArea->m_drawShapeType == PaintArea::EnumZoomShape)
//        {

//            qDebug() << "按了缩放了" <<endl;
//            m_paintArea->sX=1.1;
//            m_paintArea->sY=1.1;
//            m_paintArea->paintImage(m_paintArea->m_tmpImage);

//        }
//    }
//    if(ev->key() == Qt::Key_S)//F1按钮
//    {
//        if(m_paintArea->m_drawShapeType == PaintArea::EnumRotateShape)
//        {

//            qDebug() << "按了旋转了" <<endl;
//            m_paintArea->thita = -1;
//            m_paintArea->paintImage(m_paintArea->m_tmpImage);

//        }
//        else if(m_paintArea->m_drawShapeType == PaintArea::EnumZoomShape)
//        {
//            qDebug() << "按了缩放了" <<endl;
//            m_paintArea->sX=0.9;
//            m_paintArea->sY=0.9;
//            m_paintArea->paintImage(m_paintArea->m_tmpImage);

//        }
//    }

//}

PaintWindow::~PaintWindow()
{

}

void PaintWindow::createMenu()
{
    m_actNew = new QAction(tr("新建"), this);
    m_actNew->setShortcut(Qt::CTRL + Qt::Key_N);
    m_actOpen = new QAction(tr("打开"), this);
    m_actOpen->setShortcut(Qt::CTRL + Qt::Key_O);
    m_actSave = new QAction(tr("保存"), this);
    m_actSave->setShortcut(Qt::CTRL + Qt::Key_S);
    m_actSaveAs = new QAction(tr("另存为"), this);
    m_actExit = new QAction(tr("退出"), this);
    connect(m_actNew, SIGNAL(triggered(bool)), this, SLOT(onNew()));
    connect(m_actOpen, SIGNAL(triggered(bool)), this, SLOT(onOpen()));
    connect(m_actSave, SIGNAL(triggered(bool)), this, SLOT(onSave()));
    connect(m_actSaveAs, SIGNAL(triggered(bool)), this, SLOT(onSaveAs()));
    connect(m_actExit, SIGNAL(triggered(bool)), this, SLOT(onExit()));


    QMenu *fileMenu = new QMenu(tr("文件"), this);
    fileMenu->addAction(m_actNew);
    fileMenu->addAction(m_actOpen);
    fileMenu->addAction(m_actSave);
    fileMenu->addAction(m_actSaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(m_actExit);

    m_actZoomIn = new QAction(tr("放大"), this);
    m_actZoomOut = new QAction(tr("缩小"), this);
    m_actRestore = new QAction(tr("恢复"), this);
    m_actClear = new QAction(tr("清空"), this);
    connect(m_actZoomIn, SIGNAL(triggered(bool)), this, SLOT(onZoomIn()));
    connect(m_actZoomOut, SIGNAL(triggered(bool)), this, SLOT(onZoomOut()));
    connect(m_actRestore, SIGNAL(triggered(bool)), this, SLOT(onRestore()));
    connect(m_actClear, SIGNAL(triggered(bool)), this, SLOT(onClear()));

    QMenu *editMenu = new QMenu(tr("编辑"), this);
    editMenu->addSeparator();
    editMenu->addAction(m_actZoomIn);
    editMenu->addAction(m_actZoomOut);
    editMenu->addAction(m_actRestore);
    editMenu->addSeparator();
    editMenu->addAction(m_actClear);

    m_actLinecenter = new QAction(tr("中点算法直线"), this);
    m_actLineBresenham = new QAction(tr("Bresenham算法直线"), this);
    m_actRect = new QAction(tr("矩形"), this);
    m_actCircle = new QAction(tr("圆形"), this);
    m_actEllipse = new QAction(tr("椭圆"), this);
    m_actPolygon = new QAction(tr("多边形"), this);
    connect(m_actLinecenter, SIGNAL(triggered(bool)), this, SLOT(onDrawLinecenter()));
    connect(m_actLineBresenham, SIGNAL(triggered(bool)), this, SLOT(onDrawLineBresenham()));
    connect(m_actRect, SIGNAL(triggered(bool)), this, SLOT(onDrawRect()));
    connect(m_actCircle, SIGNAL(triggered(bool)), this, SLOT(onDrawCircle()));
    connect(m_actEllipse, SIGNAL(triggered(bool)), this, SLOT(onDrawEllipse()));
    connect(m_actPolygon, SIGNAL(triggered(bool)), this, SLOT(onDrawPolygon()));

    QMenu *toolMenu = new QMenu(tr("形状"), this);
    toolMenu->addSeparator();
    toolMenu->addAction(m_actLinecenter);
    toolMenu->addAction(m_actLineBresenham);
    toolMenu->addAction(m_actRect);
    toolMenu->addAction(m_actCircle);
    toolMenu->addAction(m_actEllipse);
    toolMenu->addAction(m_actPolygon);
    /*m_menuBar = this->menuBar();
    m_menuBar->addMenu(fileMenu);
    m_menuBar->addMenu(editMenu);
    m_menuBar->addMenu(toolMenu);*/

    m_actScan = new QAction(tr("扫描线算法"), this);
    m_actSeed = new QAction(tr("种子填充算法"), this);
    connect(m_actScan, SIGNAL(triggered(bool)), this, SLOT(onScanFill()));
    connect(m_actSeed, SIGNAL(triggered(bool)), this, SLOT(onSeedFill()));

    QMenu *fillMenu = new QMenu(tr("填充"), this);
    fillMenu->addAction(m_actScan);
    fillMenu->addAction(m_actSeed);

    m_actCohenSutherland = new QAction(tr("Cohen-Sutherland算法"), this);
    m_actCutLineCenter = new QAction(tr("中点分割裁剪算法"), this);
    connect(m_actCohenSutherland, SIGNAL(triggered(bool)), this, SLOT(onCohenSutherland()));
    connect(m_actCutLineCenter, SIGNAL(triggered(bool)), this, SLOT(onCutLineCenter()));

    QMenu *cutMenu = new QMenu(tr("裁剪"), this);
    cutMenu->addAction(m_actCohenSutherland);
    cutMenu->addAction(m_actCutLineCenter);

    m_menuBar = this->menuBar();
    m_menuBar->addMenu(fileMenu);
    m_menuBar->addMenu(editMenu);
    m_menuBar->addMenu(toolMenu);
    m_menuBar->addMenu(fillMenu);
    m_menuBar->addMenu(cutMenu);
}

void PaintWindow::createToolBar()
{
    m_toolBar = this->addToolBar(tr("ToolBar"));
    m_zoomInBtn = new QToolButton(this);
    m_zoomInBtn->setIcon(QIcon(tr(":/image/plus.png")));
    m_zoomInBtn->setToolTip(tr("放大"));
    connect(m_zoomInBtn, SIGNAL(clicked(bool)), this, SLOT(onZoomIn()));
    m_toolBar->addWidget(m_zoomInBtn);

    m_zoomOutBtn = new QToolButton(this);
    m_zoomOutBtn->setIcon(QIcon(tr(":/image/minus.png")));
    m_zoomOutBtn->setToolTip(tr("缩小"));
    connect(m_zoomOutBtn, SIGNAL(clicked(bool)), this, SLOT(onZoomOut()));
    m_toolBar->addWidget(m_zoomOutBtn);

    m_restoreBtn = new QToolButton(this);
    m_restoreBtn->setIcon(QIcon(tr(":/image/redo.png")));
    m_restoreBtn->setToolTip(tr("恢复"));
    connect(m_restoreBtn, SIGNAL(clicked(bool)), this, SLOT(onRestore()));
    m_toolBar->addWidget(m_restoreBtn);


    m_clearBtn = new QToolButton(this);
    m_clearBtn->setIcon(QIcon(tr(":/image/clear.png")));
    m_clearBtn->setToolTip(tr("清除"));
    connect(m_clearBtn, SIGNAL(clicked(bool)), this, SLOT(onClear()));
    m_toolBar->addWidget(m_clearBtn);

    m_chooseBtn = new QToolButton(this);
    m_chooseBtn->setIcon(QIcon(tr(":/image/choose.png")));
    m_chooseBtn->setToolTip(tr("选择"));
    connect(m_chooseBtn, SIGNAL(clicked(bool)), this, SLOT(onChoose()));
    m_toolBar->addWidget(m_chooseBtn);

    m_translationBtn = new QToolButton(this);
    m_translationBtn->setIcon(QIcon(tr(":/image/Move.png")));
    m_translationBtn->setToolTip(tr("平移"));
    connect(m_translationBtn, SIGNAL(clicked(bool)), this, SLOT(onTranslation()));
    m_toolBar->addWidget(m_translationBtn);

    m_rotateBtn = new QToolButton(this);
    m_rotateBtn->setIcon(QIcon(tr(":/image/rotate.png")));
    m_rotateBtn->setToolTip(tr("旋转"));
    connect(m_rotateBtn, SIGNAL(clicked(bool)), this, SLOT(onRotate()));
    m_toolBar->addWidget(m_rotateBtn);

    m_zoomBtn = new QToolButton(this);
    m_zoomBtn->setIcon(QIcon(tr(":/image/Zoom.png")));
    m_zoomBtn->setToolTip(tr("缩放"));
    connect(m_zoomBtn, SIGNAL(clicked(bool)), this, SLOT(onZoom()));
    m_toolBar->addWidget(m_zoomBtn);

    penColorBtn =new QToolButton(this);
    penColorBtn->setIcon(QIcon(tr(":/image/color.png")));
    penColorBtn->setToolTip(tr("选择颜色"));
    penColorFrame =new QFrame;
    penColorFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    penColorFrame->setAutoFillBackground(true);
    penColorFrame->setPalette(QPalette(Qt::black));
    connect(penColorBtn,SIGNAL(clicked()),this,SLOT(ShowPenColor()));

    m_toolBar->addWidget(penColorBtn);

    penWidthLabel =new QLabel(tr("画笔线宽：")); 		//画笔线宽选择控件
    penWidthSpinBox =new QSpinBox;
    penWidthSpinBox->setRange(1,20);
    connect(penWidthSpinBox,SIGNAL(valueChanged(int)),this,SLOT (ShowPenWidth (int)));

    penStyleLabel =new QLabel(tr("画笔风格："));	 	//画笔风格选择下拉列表框
    penStyleComboBox =new QComboBox;
    penStyleComboBox->addItem(tr("实线"),					//(b)
                                    static_cast<int>(Qt::SolidLine));
    penStyleComboBox->addItem(tr("虚线"),
                                    static_cast<int>(Qt::DashLine));
    connect(penStyleComboBox,SIGNAL(activated(int)),this,SLOT (ShowPenStyle (int)));

    m_toolBar->addWidget(penStyleLabel);
    m_toolBar->addWidget(penStyleComboBox);

    m_toolBar->addWidget(penWidthLabel);
    m_toolBar->addWidget(penWidthSpinBox);



}

void PaintWindow::closeEvent(QCloseEvent */*event*/)
{
    //在关闭主窗口时，先判断图片是否被更改过
    if (m_paintArea->getIsModify()) {
       int ret = QMessageBox::warning(this, tr(""), tr("是否保存图片？"), QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            this->onSave();
        }
    }

    this->close();
}

void PaintWindow::onNew()
{
    m_paintArea->setImageSize(m_paintArea->size());
    m_paintArea->setImageBgColor(QColor(255, 255, 255));
}

void PaintWindow::onOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), tr(""), tr("PNG(*.png)\n JPEG(*.jpg)\n BMP(*.bmp)"));
    if (fileName.isEmpty())
        return;

    m_paintArea->setImage(fileName);
    m_paintArea->resize(m_paintArea->getImageSize());

    m_curFile = fileName;
}

void PaintWindow::onSave()
{
    QImage image = m_paintArea->getImage();
    image.save(m_curFile);                   //m_curFile默认路径
}

void PaintWindow::onSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::currentPath(), tr("PNG(*.png)\n BMP(*.bmp)\n JPEG(*.jpeg)"));
    if (fileName.isEmpty())
        return;

    QImage image = m_paintArea->getImage();
    image.save(fileName);
}

void PaintWindow::onExit()
{
    this->close();
}

void PaintWindow::onZoomIn()
{
    m_paintArea->zoomIn();
    m_paintArea->resize(m_paintArea->getImageSize());
}

void PaintWindow::onZoomOut()
{
    m_paintArea->zoomOut();
}

void PaintWindow::onRestore()
{
    m_paintArea->restore();
}

void PaintWindow::onClear()
{
    m_paintArea->clear();
}

void PaintWindow::onChoose()
{
    m_paintArea->choose();
}

void PaintWindow::onTranslation()
{
    m_paintArea->setDrawShape(PaintArea::EnumTransShape);
}

void PaintWindow::onRotate()
{
    m_paintArea->setDrawShape(PaintArea::EnumRotateShape);
}

void PaintWindow::onZoom()
{
    m_paintArea->setDrawShape(PaintArea::EnumZoomShape);
}

void PaintWindow::onDrawNone()
{
    m_paintArea->setDrawShape(PaintArea::EnumDrawNone);
}

void PaintWindow::onDrawLinecenter()
{
    m_paintArea->setDrawShape(PaintArea::EnumDrawLinecenter);
}

void PaintWindow::onDrawLineBresenham()
{
    m_paintArea->setDrawShape(PaintArea::EnumDrawLineBresenham);
}


void PaintWindow::onDrawRect()
{
    m_paintArea->setDrawShape(PaintArea::EnumDrawRect);
}

void PaintWindow::onDrawCircle()
{
    m_paintArea->setDrawShape(PaintArea::EnumDrawCircle);
}

void PaintWindow::onDrawEllipse()
{
    m_paintArea->setDrawShape(PaintArea::EnumDrawEllipse);
}

void PaintWindow::onDrawPolygon()
{
    m_paintArea->setDrawShape(PaintArea::EnumDrawPolygon);
}

void PaintWindow::onScanFill()
{
    m_paintArea->setDrawShape(PaintArea::EnumScanFill);
}

void PaintWindow::onSeedFill()
{
    m_paintArea->setDrawShape(PaintArea::EnumSeedFill);
}

void PaintWindow::onCohenSutherland(){
    m_paintArea->setDrawShape(PaintArea::EnumCohenSutherland);
}

void PaintWindow::onCutLineCenter(){
    m_paintArea->setDrawShape(PaintArea::EnumCutLineCenter);
}

void PaintWindow::onFillRectangle(){
    m_paintArea->setDrawShape(PaintArea::EnumFillRectangle);
}

void PaintWindow::onDrawDashLine()
{
    m_paintArea->setLineShape(PaintArea::dashLine);
}

void PaintWindow::onDrawDefaultLine()
{
    m_paintArea->setLineShape(PaintArea::defaultLine);
}

void PaintWindow::ShowPenColor()
{
 QColor color = QColorDialog::getColor(static_cast<int>(Qt::black));
 int value = penWidthSpinBox->value();
 Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(
     penStyleComboBox->currentIndex(),Qt::UserRole).toInt());

 penColorFrame->setPalette(QPalette(color));

 m_paintArea->setPen(QPen(color,1,style),value);
}

void PaintWindow::ShowPenWidth(int value)
{
    QColor color = penColorFrame->palette().color(QPalette::Window);
 Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(
        penStyleComboBox->currentIndex(),Qt::UserRole).toInt());

 m_paintArea->setPen(QPen(color,1,style),value);

}

void PaintWindow::ShowPenStyle(int styleValue)
{QColor color = penColorFrame->palette().color(QPalette::Window);
 int value = penWidthSpinBox->value();
 Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(
        styleValue,Qt::UserRole).toInt());

 if(penStyleComboBox->currentIndex() == 1)
     onDrawDashLine();
 else if (penStyleComboBox->currentIndex() == 0)
     onDrawDefaultLine();

 m_paintArea->setPen(QPen(color,1,style),value);
}


