#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
import os
from PyQt4 import QtGui
from PyQt4 import QtCore

from widget import GLWidget


class Tool(QtGui.QMainWindow):

    def __init__(self):
        super(Tool, self).__init__()
        self.initUI()

    def initUI(self):
        self.setGeometry(50, 50, 1024, 700)
        self.setWindowTitle('Tool')

        openAction = QtGui.QAction(QtGui.QIcon('icons/openfile.png'), 'Open', self)
        openAction.setShortcut('Ctrl+O')
        openAction.setStatusTip('Open .obj file')
        openAction.triggered.connect(self.showFileDialog)
        quitAction = QtGui.QAction(QtGui.QIcon('icons/quit.png'), 'Exit', self)
        quitAction.setShortcut('Ctrl+Q')
        quitAction.setStatusTip('Exit application')
        quitAction.triggered.connect(self.close)
        menu = self.menuBar().addMenu('&Menu')
        menu.addAction(openAction)
        menu.addAction(quitAction)
        menubar = QtGui.QToolBar()
        self.addToolBar(QtCore.Qt.TopToolBarArea, menubar)
        menubar.addAction(openAction)
        menubar.addAction(quitAction)

        lightAction = QtGui.QAction(QtGui.QIcon('icons/lighted.png'), 'Light', self)
        lightAction.triggered.connect(self.eventManager)
        textureAction = QtGui.QAction(QtGui.QIcon('icons/texture.png'), 'Texture', self)
        textureAction.triggered.connect(self.eventManager)
        rotationAction = QtGui.QAction(QtGui.QIcon('icons/rotation.png'), 'Rotation', self)
        rotationAction.triggered.connect(self.eventManager)
        translationAction = QtGui.QAction(QtGui.QIcon('icons/translation.png'), 'Translation', self)
        translationAction.triggered.connect(self.eventManager)
        scaleAction = QtGui.QAction(QtGui.QIcon('icons/scale.png'), 'Scale', self)
        scaleAction.triggered.connect(self.eventManager)
        subdivisionAction = QtGui.QAction(QtGui.QIcon('icons/subdivision.png'), 'Subdivision', self)
        subdivisionAction.triggered.connect(self.eventManager)
        showlineAction = QtGui.QAction(QtGui.QIcon('icons/showline.png'), 'ShowLine', self)
        showlineAction.triggered.connect(self.eventManager)
        edit = self.menuBar().addMenu('&Edit')
        edit.addAction(lightAction)
        edit.addAction(textureAction)
        edit.addAction(rotationAction)
        edit.addAction(translationAction)
        edit.addAction(scaleAction)
        edit.addAction(subdivisionAction)
        edit.addAction(showlineAction)
        editbar = QtGui.QToolBar()
        self.addToolBar(QtCore.Qt.TopToolBarArea, editbar)
        editbar.addAction(lightAction)
        editbar.addAction(textureAction)
        editbar.addAction(rotationAction)
        editbar.addAction(translationAction)
        editbar.addAction(scaleAction)
        editbar.addAction(subdivisionAction)
        editbar.addAction(showlineAction)

        mainWidget = QtGui.QWidget(self)

        leftWidget = QtGui.QWidget(mainWidget)
        leftWidget.setGeometry(10, 0, 150, 600)

        scaleGroup = QtGui.QGroupBox('scale control', leftWidget)
        scaleGroup.setGeometry(0, 0, 150, 120)
        scaleLayout = QtGui.QVBoxLayout(scaleGroup)
        self.scaleSliderX = QtGui.QSlider(QtCore.Qt.Horizontal)
        self.scaleSliderX.setRange(1, 100)
        self.scaleSliderX.setValue(50)
        self.scaleSliderX.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.scaleSliderX.valueChanged[int].connect(self.sliderChanged)
        self.scaleSliderY = QtGui.QSlider(QtCore.Qt.Horizontal)
        self.scaleSliderY.setRange(1, 100)
        self.scaleSliderY.setValue(50)
        self.scaleSliderX.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.scaleSliderY.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.scaleSliderY.valueChanged[int].connect(self.sliderChanged)
        self.scaleSliderZ = QtGui.QSlider(QtCore.Qt.Horizontal)
        self.scaleSliderZ.setRange(1, 100)
        self.scaleSliderZ.setValue(50)
        self.scaleSliderZ.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.scaleSliderZ.valueChanged[int].connect(self.sliderChanged)
        self.scaleSliderX.setFocusPolicy(QtCore.Qt.NoFocus)
        self.scaleSliderY.setFocusPolicy(QtCore.Qt.NoFocus)
        self.scaleSliderZ.setFocusPolicy(QtCore.Qt.NoFocus)
        scaleLayout.addWidget(self.scaleSliderX)
        scaleLayout.addWidget(self.scaleSliderY)
        scaleLayout.addWidget(self.scaleSliderZ)
        scaleLayout.addStretch(1)

        rotateGroup = QtGui.QGroupBox('rotate control', leftWidget)
        rotateGroup.setGeometry(0, 0, 150, 120)
        rotateLayout = QtGui.QVBoxLayout(rotateGroup)
        self.rotateSlider = QtGui.QSlider(QtCore.Qt.Horizontal)
        self.rotateSlider.setRange(-360, 360)
        self.rotateSlider.setValue(0)
        self.rotateSlider.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.rotateSlider.valueChanged[int].connect(self.sliderChanged)
        self.rotateSlider.setFocusPolicy(QtCore.Qt.NoFocus)
        self.rotateX = QtGui.QPushButton('RotateX', self)
        self.rotateX.setCheckable(True)
        self.rotateY = QtGui.QPushButton('RotateY', self)
        self.rotateY.setCheckable(True)
        self.rotateZ = QtGui.QPushButton('RotateZ', self)
        self.rotateZ.setCheckable(True)
        rotateLayout.addWidget(self.rotateSlider)
        rotateLayout.addWidget(self.rotateX)
        rotateLayout.addWidget(self.rotateY)
        rotateLayout.addWidget(self.rotateZ)
        rotateLayout.addStretch(1)

        translateGroup = QtGui.QGroupBox('translate control', leftWidget)
        translateGroup.setGeometry(0, 0, 150, 120)
        translateLayout = QtGui.QVBoxLayout(translateGroup)
        self.translateSliderX = QtGui.QSlider(QtCore.Qt.Horizontal)
        self.translateSliderX.setRange(-50, 50)
        self.translateSliderX.setValue(0)
        self.translateSliderX.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.translateSliderX.valueChanged[int].connect(self.sliderChanged)
        self.translateSliderY = QtGui.QSlider(QtCore.Qt.Horizontal)
        self.translateSliderY.setRange(-50, 50)
        self.translateSliderY.setValue(0)
        self.translateSliderX.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.translateSliderY.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.translateSliderY.valueChanged[int].connect(self.sliderChanged)
        self.translateSliderZ = QtGui.QSlider(QtCore.Qt.Horizontal)
        self.translateSliderZ.setRange(-50, 50)
        self.translateSliderZ.setValue(0)
        self.translateSliderZ.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.translateSliderZ.valueChanged[int].connect(self.sliderChanged)
        self.translateSliderX.setFocusPolicy(QtCore.Qt.NoFocus)
        self.translateSliderY.setFocusPolicy(QtCore.Qt.NoFocus)
        self.translateSliderZ.setFocusPolicy(QtCore.Qt.NoFocus)
        translateLayout.addWidget(self.translateSliderX)
        translateLayout.addWidget(self.translateSliderY)
        translateLayout.addWidget(self.translateSliderZ)
        translateLayout.addStretch(1)

        leftLayout = QtGui.QVBoxLayout(leftWidget)
        leftLayout.addWidget(scaleGroup)
        leftLayout.addWidget(rotateGroup)
        leftLayout.addWidget(translateGroup)
        leftLayout.addStretch(1)

        rightWidget = QtGui.QWidget(mainWidget)
        rightWidget.setGeometry(180, 0, 800, 600)
        self.gl = GLWidget()
        rightLayout = QtGui.QVBoxLayout(rightWidget)
        rightLayout.addWidget(self.gl)

        self.setCentralWidget(mainWidget)

        self.statusBar().showMessage('Init Done')
        self.setFocus()
        
    def keyPressEvent(self, e):
        if e.key() == QtCore.Qt.Key_Space:
            self.gl.paint_lists()
            self.statusBar().showMessage('Paint Lists')
        elif e.key() == QtCore.Qt.Key_W:
            self.gl.change(1)
            self.statusBar().showMessage('W')
        elif e.key() == QtCore.Qt.Key_S:
            self.gl.change(2)
            self.statusBar().showMessage('S')
        elif e.key() == QtCore.Qt.Key_A:
            self.gl.change(3)
            self.statusBar().showMessage('A')
        elif e.key() == QtCore.Qt.Key_D:
            self.gl.change(4)
            self.statusBar().showMessage('D')

    def eventManager(self):
        eve = self.sender().text()
        if eve == 'Light':
            if self.gl.light_toggle():
                self.statusBar().showMessage('Lighted')
            else:
                self.statusBar().showMessage('No Lighted')
        elif eve == 'Rotation':
            pass
        elif eve == 'Translation':
            pass
        elif eve == 'Scale':
            pass
        elif eve == 'Texture':
            if self.gl.show_texture_toggle():
                self.statusBar().showMessage('Show Texture')
            else:
                self.statusBar().showMessage('No Show Texture')
        elif eve == 'ShowLine':
            if self.gl.show_line_toggle():
                self.statusBar().showMessage('Show Line')
            else:
                self.statusBar().showMessage('No Show Line')
        elif eve == 'Subdivision':
            self.gl.loop_subdivision()
            self.statusBar().showMessage('Subdivision')

    def sliderChanged(self, value):
        eve = self.sender()
        if eve == self.scaleSliderX:
            self.gl.scale(0, value)
            self.statusBar().showMessage('Scale axis: X ' + str(value/50.) + ' times')
        elif eve == self.scaleSliderY:
            self.gl.scale(1, value)
            self.statusBar().showMessage('Scale axis: Y ' + str(value/50.) + ' times')
        elif eve == self.scaleSliderZ:
            self.gl.scale(2, value)
            self.statusBar().showMessage('Scale axis: Z ' + str(value/50.) + ' times')
        elif eve == self.translateSliderX:
            self.gl.translate(0, value)
            self.statusBar().showMessage('Translate axis: X ' + str(value/5.) + ' units')
        elif eve == self.translateSliderY:
            self.gl.translate(1, value)
            self.statusBar().showMessage('Translate axis: Y ' + str(value/5.) + ' units')
        elif eve == self.translateSliderZ:
            self.gl.translate(2, value)
            self.statusBar().showMessage('Translate axis: Z ' + str(value/5.) + ' units')
        elif eve == self.rotateSlider:
            rotation = [0., 0., 0., 0.]
            rotation[0] = value
            if self.rotateX.isChecked():
                rotation[1] = 1.
            else:
                rotation[1] = 0.
            if self.rotateY.isChecked():
                rotation[2] = 1.
            else:
                rotation[2] = 0.
            if self.rotateZ.isChecked():
                rotation[3] = 1.
            else:
                rotation[3] = 0.
            self.gl.rotate(rotation)
            self.statusBar().showMessage('Rotate: ' + str(rotation))


    def showFileDialog(self):
        filename = QtGui.QFileDialog.getOpenFileName(self, 'Open .obj file',
                                              os.getcwd() + '/model/')
        if len(filename) != 0 and os.path.isfile(filename):
            self.statusBar().showMessage('Loading')
            self.gl.loadobj(filename)
            self.statusBar().showMessage('Load Done')
            self.gl.paint_lists()


if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    ex = Tool()
    ex.show()
    app.exec_()
