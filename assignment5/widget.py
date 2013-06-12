#!/usr/bin/python
# -*- coding: utf-8 -*-
from PyQt4 import QtCore
from PyQt4 import QtOpenGL
from OpenGL.GL import *
from OpenGL.GLU import *
import threading
import math
from entity import Entity
from entity import Coordinate

class GLManager(object):
    ROTATE = 1. / 48. * math.acos(-1)

    def __init__(self, parent = None):
        self.eye, self.target, self.up = [0, 0, 9], [0, 0, 0], [0, 1, 0]
        self.radius, self.theta = 3., 0.
        self.lighted = False
        light_ambient = [0., 0., 0., 1.]
        light_diffuse = [1., 1., 1., 1.]
        light_specular = [1., 1., 1., 1.]
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse)
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular)
        glLightfv(GL_LIGHT0, GL_POSITION, self.eye)

    def init(self):
        glClearColor(0.0, 0.0, 0.0, 1.0)
        glClear(GL_COLOR_BUFFER_BIT)
        glShadeModel(GL_SMOOTH)
        glEnable(GL_DEPTH_TEST)

    def clear(self):
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)

    def resize(self, w, h):
        self.width, self.height = w, h
        glViewport(0, 0, w, h)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(100, w/h, .1, 100)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        gluLookAt(*self.getlookat())

    def update(self):
        glViewport(0, 0, self.width, self.height)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(100, self.width/self.height, .1, 100)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        gluLookAt(*self.getlookat())

    def getlookat(self):
        return self.eye + self.target + self.up

    def light_toggle(self):
        if self.lighted:
            glDisable(GL_LIGHT0)
            glDisable(GL_LIGHTING)
        else:
            glEnable(GL_LIGHTING)
            glEnable(GL_LIGHT0)
        self.lighted = not self.lighted
        return self.lighted

    def zoom_view(self, selected):
        if selected:
            self.radius += .2
        else:
            self.radius -= .2
        self.eye[0] = self.radius * math.sin(self.theta)
        self.eye[1] = 0.
        self.eye[2] = self.radius * math.cos(self.theta)

    def move_view(self, selected):
        if selected:
            self.theta += self.ROTATE
        else:
            self.theta -= self.ROTATE
        self.eye[0] = self.radius * math.sin(self.theta)
        self.eye[1] = 0.
        self.eye[2] = self.radius * math.cos(self.theta)

class GLWidget(QtOpenGL.QGLWidget):
    def __init__(self, parent = None):
        super(GLWidget, self).__init__(parent)
        self.manager = GLManager(self)
        self.coordinate = Coordinate()
        self.entity = None

    def loadobj(self, filename):
        self.entity = Entity()
        self.entity.load_obj(filename)
#threading.Thread(target=self.entity.load_obj, args=(filename, ),
#name='loadobj').start()  

    def paintGL(self):
        self.manager.clear()
        self.coordinate.draw()
        if self.entity is not None:
            self.entity.draw()
        self.swapBuffers()

    def resizeGL(self, w, h):
        self.width, self.height = w, h
        self.setGeometry(QtCore.QRect(0, 0, w, h))
        self.manager.resize(w, h)

    def initializeGL(self):
        self.manager.init()

    def update(self):
        self.manager.update()
        self.updateGL()

    def change(self, selected):
        if selected == 1:
            self.manager.zoom_view(0)
        elif selected == 2:
            self.manager.zoom_view(1)
        elif selected == 3:
            self.manager.move_view(0)
        elif selected == 4:
            self.manager.move_view(1)
        self.update()

    def light_toggle(self):
        res = self.manager.light_toggle()
        self.update()
        return res

    def show_line_toggle(self):
        res = self.entity.show_line_toggle()
        self.update()
        return res

    def show_texture_toggle(self):
        res = self.entity.show_texture_toggle()
        self.update()
        return res

    def loop_subdivision(self):
        self.entity.gen_half_edge_structure()
        self.entity.loop_subdivision()
        self.entity.display_subdivision_toggle()
        self.update()

    def paint_lists(self):
        if self.entity is not None:
            self.entity.display_lists_toggle()
        self.update()

    def rotate(self, value):
        if self.entity is not None:
            self.entity.set_rotation(value)
        self.update()

    def scale(self, axis, value):
        if self.entity is not None:
            self.entity.set_scale(axis, value)
        self.update()

    def translate(self, axis, value):
        if self.entity is not None:
            self.entity.set_translation(axis, value)
        self.update()
