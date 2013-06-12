# !/usr/bin/env python
# -*- coding: utf-8 -*-

"""
    Copyright (c)2013, Legend Lee * All rightsreserve
    Use: Simulator for Suzaku Quadrotor
    Module: Simulator Object Loader
    Author: Legend Lee
    Date: 2013-04-16
    E-mail: legendlee1314@gmail.com
"""
try:
    from OpenGL.GL import *
    import pygame
except ImportError:
    print "%s at objloader module!" % ImportError

# load .mtl file
def load_mtl(filename):
    """
        Read file of .mtl.
        Return the contents.
    """
    assert filename is not None
    contents, mtl = {}, None
    for line in open(filename, 'r'):
        if line.startswith('#'):
            continue
        values = line.split()
        if not values:
            continue
        if values[0] == "newmtl":
            mtl = contents[values[1]] = {}
        elif mtl is None:
            raise ValueError(".mtl file doesn't start with newmtl")
        elif values[0] == "map_Kd":
            # load the texture referred to by this declaration
            mtl[values[0]] = values[1]
            surf = pygame.image.load(mtl['map_Kd'])
            image = pygame.image.tostring(surf, 'RGBA', 1)
            ix, iy = surf.get_rect().size
            texid = mtl['texture_Kd'] = glGenTextures(1)
            glBindTexture(GL_TEXTURE_2D, texid)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                            GL_LINEAR)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                            GL_MIRRORED_REPEAT)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                            GL_MIRRORED_REPEAT)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ix, iy, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, image)
        else:
            # load the other values like color
            mtl[values[0]] = map(float, values[1:])
    return contents


class OBJLoader(object):
    """
        Loads a Wavefront OBJ file.
        Generate the GLlists by OpenGL.
        Then call getGLlists() to return it.
    """

    def __init__(self, filename):
        """
            @filename consists of path + name.
        """
        assert filename is not None
        self.filename = filename
        self.vertices, self.normals = [], []
        self.texcoords, self.faces = [], []
        self.mtl, self.faceList = {}, []

    def load(self):
        """
            @filename consists of path + name.
        """
        assert self.filename is not None
        material = None
        for line in open(self.filename, 'r'):
            if line.startswith('#'):
                continue
            values = line.split()
            if not values:
                continue
            if values[0] == 'v':
                v = map(float, values[1:4])
                self.vertices.append(v)
            elif values[0] == 'vn':
                v = map(float, values[1:4])
                self.normals.append(v)
            elif values[0] == 'vt':
                self.texcoords.append(map(float, values[1:3]))
            elif values[0] in ("usemtl", "usemat"):
                material = values[1]
            elif values[0] == "mtllib":
                self.mtl = load_mtl(values[1])
            elif values[0] == 'f':
                face, texcoords, norms, vList = [], [], [], []
                for v in values[1:]:
                    w = v.split('/')
                    face.append(int(w[0]))
                    vList.append(int(w[0]) - 1)
                    if len(w) >= 2 and len(w[1]) > 0:
                        texcoords.append(int(w[1]))
                    else:
                        texcoords.append(0)
                    if len(w) >= 3 and len(w[2]) > 0:
                        norms.append(int(w[2]))
                    else:
                        norms.append(0)
                self.faceList.append(vList)
                self.faces.append((face, norms, texcoords, material))

        return True


# local test
if __name__ == '__main__':
    obj = OBJLoader('./model/suzaku.obj')
    obj.load()
