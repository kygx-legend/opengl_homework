#!/usr/bin/python
# -*- coding: utf-8 -*-

try:
    from OpenGL.GL import *
    from OpenGL.GLU import *
    from objloader import *
except ImportError:
    print "%s at entity module!" % ImportError

# Half Edge Structure
class HE_vert():
    def __init__(self, i, pos, edge):
        self.i, self.pos, self.edge = i, pos, edge

class HE_edge():
    def __init__(self, _origine, _face, _next, _prev):
        self.origine_ = _origine
        self.face_ = _face
        self.next_ = _next
        self.prev_ = _prev
        self.twin_ = None

class HE_face():
    def __init__(self, i, edge):
        self.i, self.edge = i, edge

# Entity class
class Entity(object):
    """
        Entity module to construct one object in a world
    """
    def __init__(self):
        self.obj =  None
        self.display_lists = False
        self.display_subdivision = False
        self.showline = False
        self.gl_lists = None
        self.scale = [1., 1., 1.]
        self.rotation = [0., 0., 0., 0.]
        self.translation = [0., 0., 0.]
        self.showtexture = True
        # half edge structure
        self.halfEdgeVert = []
        self.halfEdgeList = []
        self.halfEdgeFace = []
        self.newfaces = []

    def __del__(self):
        if self.gl_lists is not None:
            glDeleteLists(self.gl_lists, 1)

    def draw(self):
        """
            Draw itself at OpenGL.
        """
        glPushMatrix()
        glTranslatef(*self.translation)
        glRotatef(*self.rotation)
        glScalef(*self.scale)
        self.call_lists()
        self.draw_subdivision()
        glPopMatrix()

    def load_obj(self, filename):
        """
            Load obj file.
            @filename consists of path + name.
        """
        assert filename is not None
        self.obj = OBJLoader(filename)
        self.obj.load()
        self.gen_gl_lists()
        print '=====> %s loaded successfully!' % filename

    def gen_gl_lists(self):
        """
            Generate GLlists of the object of file
        """
        self.gl_lists = glGenLists(1)
        glNewList(self.gl_lists, GL_COMPILE)
        glEnable(GL_TEXTURE_2D)
        glFrontFace(GL_CCW)
        for face in self.obj.faces:
            vertices, normals, texture_coords, material = face
            if len(self.obj.mtl) != 0:
                mtl = self.obj.mtl[material]
                if "texture_Kd" in mtl:
                    # use diffuse texmap
                    glBindTexture(GL_TEXTURE_2D, mtl['texture_Kd'])
                else:
                    # just use diffuse colour
                    glColor(*mtl['Kd'])
            if self.showline:
                glBegin(GL_LINE_LOOP)
            else:
                glBegin(GL_POLYGON)
            for i in range(len(vertices)):
                if normals[i] > 0:
                    glNormal3f(*self.obj.normals[normals[i] - 1])
                if texture_coords[i] > 0 and self.showtexture:
                    glTexCoord2f(*self.obj.texcoords[texture_coords[i] - 1])
                glVertex3f(*self.obj.vertices[vertices[i] - 1])
            glEnd()
        glDisable(GL_TEXTURE_2D)
        glEndList()

    def call_lists(self):
        """
            Call self.display_lists at OpenGL.
            It's the same as draw().
        """
        if self.display_lists:
            glCallList(self.gl_lists)

    def draw_subdivision(self):
        if self.display_subdivision:
            self.gl_lists_sub = glGenLists(1)
            glNewList(self.gl_lists_sub, GL_COMPILE)
            glFrontFace(GL_CCW)
            for f in self.newfaces:
                glColor((0., 0., 0., 1.))
                glLineWidth(2.0)
                glBegin(GL_LINE_LOOP)
                for v in f:
                    glVertex3f(*v)
                glEnd()
                glColor((1., 1., 1., 1.))
                glBegin(GL_POLYGON)
                for v in f:
                    glVertex3f(*v)
                glEnd()
            glEndList()
            glCallList(self.gl_lists_sub)

    def gen_half_edge_structure(self):
        self.makeHalfEdgeWithOrigineAndFace()
        self.setHalfEdgeWithTwin()
        self.setHalfEdgeWithNextAndPrev()
        self.setHalfEdgeVert()
        self.setHalfEdgeFace()
        self.printHalfEdgeList()
  
    def makeHalfEdgeWithOrigineAndFace(self):
        self.halfEdgeList = []
        for i in range(0, len(self.obj.faceList)):
            for tail in self.obj.faceList[i]:
                newHe = HE_edge(tail, i, -1, -1)
                self.halfEdgeList.append(newHe)

    def setHalfEdgeWithTwin(self):
        edgeMap = {}
        for i in range(0, len(self.obj.faceList)):
            for j in range(0, len(self.obj.faceList[i])):
                tail = self.obj.faceList[i][j]
                head = -1
                if j + 1 == len(self.obj.faceList[i]):
                    head = self.obj.faceList[i][0]
                else:
                    head = self.obj.faceList[i][j+1]
                key = tail, head
                for k in range(0, len(self.halfEdgeList)):
                    if self.halfEdgeList[k].origine_ == tail \
                       and self.halfEdgeList[k].face_ == i:
                        edgeMap[key] = k

        for i in edgeMap:
            j = i[1], i[0]
            curEdgeIndex = edgeMap[i]
            twinEdgeIndex = edgeMap[j]
            self.halfEdgeList[curEdgeIndex].twin_ = twinEdgeIndex
            self.halfEdgeList[twinEdgeIndex].twin_ = curEdgeIndex

    def setHalfEdgeWithNextAndPrev(self):
        for i in range(0, len(self.obj.faceList)):
            f = self.obj.faceList[i]
            edges = []
            for j in f:
                for k in range(0, len(self.halfEdgeList)):
                    if self.halfEdgeList[k].origine_ == j and \
                       self.halfEdgeList[k].face_ == i:
                        edges.append(k)
            for j in range(0, len(edges)):
                if j != len(edges) - 1:
                    self.halfEdgeList[edges[j]].next_ = edges[j + 1]
                    self.halfEdgeList[edges[j + 1]].prev_ = edges[j]
                else:
                    self.halfEdgeList[edges[j]].next_ = edges[0]
                    self.halfEdgeList[edges[0]].prev_ = edges[j]

    def setHalfEdgeVert(self):
        for i in xrange(0, len(self.halfEdgeList)):
            he = self.halfEdgeList[i]
            pos = self.obj.vertices[he.origine_]
            self.halfEdgeVert.append(HE_vert(he.origine_, pos, i))

    def setHalfEdgeFace(self):
        for i in xrange(0, len(self.halfEdgeList)):
            he = self.halfEdgeList[i]
            self.halfEdgeFace.append(HE_face(he.face_, i))

    def printHalfEdgeList(self):
        outStr = ''
        k = 0
        for i in self.halfEdgeList:
            k += 1
            outStr += str(k)
            outStr += ' '
            outStr += str(i.origine_ + 1)
            outStr += ' '
            outStr += str(i.twin_ + 1)
            outStr += ' '
            outStr += str(i.face_ + 1)
            outStr += ' '
            outStr += str(i.next_ + 1)
            outStr += ' '
            outStr += str(i.prev_ + 1)
            outStr += '\n'
        print(outStr)

    def loop_subdivision(self):
        self.newfaces = []
        if len(self.halfEdgeList) > 0:
            for f in self.halfEdgeFace:
                he = self.halfEdgeList[f.edge]
                p1 = self.loop_subdivision_gen(he)
                p2 = self.loop_subdivision_gen(self.halfEdgeList[he.next_])
                p3 = self.loop_subdivision_gen(self.halfEdgeList[he.prev_])
                if p1 is not None and p2 is not None and p3 is not None:
                    self.newfaces.append((p1[2], p2[4], p3[4]))
                    self.newfaces.append((p1[0], p1[4], p2[4]))
                    self.newfaces.append((p1[4], p2[4], p3[4]))
                    self.newfaces.append((p1[4], p1[1], p2[4]))

    def loop_subdivision_gen(self, he):
        p0 = self.obj.vertices[he.origine_]
        p1 = self.obj.vertices[self.halfEdgeList[he.next_].origine_]
        p2 = self.obj.vertices[self.halfEdgeList[he.prev_].origine_]
        if he.twin_ is not None:
            p3 = self.obj.vertices[self.halfEdgeList[he.twin_].origine_]
            func = lambda (x0, x1, x2, x3): 1./8. * (x2 + x3) + 3./8. * (x0 + x1)
            np = map(func, zip(p0, p1, p2, p3))
            np0 = [5./8. * x0 + 3./8. * x1 for (x0, x1) in zip(p0, self.get_average(he.origine_))]
            np1 = [5./8. * x0 + 3./8. * x1 for (x0, x1) in zip(p0, self.get_average(self.halfEdgeList[he.next_].origine_))]
            np2 = [5./8. * x0 + 3./8. * x1 for (x0, x1) in zip(p0, self.get_average(self.halfEdgeList[he.prev_].origine_))]
            np3 = [5./8. * x0 + 3./8. * x1 for (x0, x1) in zip(p0, self.get_average(self.halfEdgeList[he.twin_].origine_))]
        else:
            print 'a'
            return None
        return (np0, np1, np2, np3, np)

    def get_average(self, he_v):
        total, num = [0., 0., 0.], 0.
        for e in self.halfEdgeList:
            if e.origine_ == he_v:
                total[0] += self.obj.vertices[self.halfEdgeList[e.next_].origine_][0]
                total[1] += self.obj.vertices[self.halfEdgeList[e.next_].origine_][1]
                total[2] += self.obj.vertices[self.halfEdgeList[e.next_].origine_][2]
                num += 1.
        return [t / num for t in total]

    def display_lists_toggle(self):
        self.display_lists = not self.display_lists

    def display_subdivision_toggle(self):
        self.display_subdivision = not self.display_subdivision

    def show_line_toggle(self):
        if self.gl_lists is not None:
            glDeleteLists(self.gl_lists, 1)
        self.showline = not self.showline
        self.gen_gl_lists()
        return self.showline

    def show_texture_toggle(self):
        if self.gl_lists is not None:
            glDeleteLists(self.gl_lists, 1)
        self.showtexture = not self.showtexture
        self.gen_gl_lists()
        return self.showtexture

    def set_rotation(self, value):
        self.rotation = value

    def set_translation(self, axis, value):
        self.translation[axis] = value / 5.

    def set_scale(self, axis, value):
        self.scale[axis] = value / 50.

class Coordinate(object):
    """
        Entity module to construct one object in a world
    """
    def __init__(self):
        self.origin = [0, 0, 0]
        self.axisX = [20, 0, 0]
        self.axisXC = [1, 0, 0]
        self.axisY = [0, 20, 0]
        self.axisYC = [0, 1, 0]
        self.axisZ = [0, 0, 20]
        self.axisZC = [0, 0, 1]

    def draw(self):
        """
            Draw itself at OpenGL.
        """
        glPushMatrix()
        glBegin(GL_LINES)
        glColor3f(*self.axisXC)
        glVertex3f(*self.origin)
        glVertex3f(*self.axisX)
        glEnd()
        glBegin(GL_LINES)
        glColor3f(*self.axisYC)
        glVertex3f(*self.origin)
        glVertex3f(*self.axisY)
        glEnd()
        glBegin(GL_LINES)
        glColor3f(*self.axisZC)
        glVertex3f(*self.origin)
        glVertex3f(*self.axisZ)
        glEnd()
        glPopMatrix()

if __name__ == '__main__':
    entity = Entity()
