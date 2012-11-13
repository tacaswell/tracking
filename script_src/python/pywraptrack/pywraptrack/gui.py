#Copyright 2012 Thomas A Caswell
#tcaswell@uchicago.edu
#http://jfi.uchicago.edu/~tcaswell
#
#This program is free software; you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation; either version 3 of the License, or (at
#your option) any later version.
#
#This program is distributed in the hope that it will be useful, but
#WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program; if not, see <http://www.gnu.org/licenses>.


# do this to make me learn where stuff is and to make it easy to switch to PyQt later
import PySide.QtCore as QtCore
import PySide.QtGui as QtGui

import matplotlib
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt4agg import NavigationToolbar2QTAgg as NavigationToolbar
from matplotlib.figure import Figure

import numpy as np
from .swig_wrappers import *

class IdenGui(QtGui.QMainWindow):
     
    spinner_lst = [
        {'name':'p_rad','min':0,'max':50,'step':1,'type':np.int,'default':4},
        {'name':'d_rad','min':0,'max':50,'step':1,'type':np.int,'default':3},
        {'name':'mask_rad','min':0,'max':50,'step':1,'type':np.int,'default':5},
        
        {'name':'top_cut','min':0,'max':1,'step':.001,'prec':4,'type':np.float,'default':.001},
        {'name':'threshold','min':0,'max':100,'step':1,'prec':1,'type':np.float,'default':1.0},
        {'name':'hwhm','min':0,'max':50,'step':.1,'prec':1,'type':np.float,'default':1.3},

        {'name':'shift_cut','min':0,'max':5,'step':.1,'prec':1,'type':np.float,'default':1.5},
        {'name':'rg_cut','min':0,'max':100,'step':1,'prec':1,'type':np.float,'default':10},
        {'name':'e_cut','min':0,'max':1,'step':.1,'prec':2,'type':np.float,'default':.5},

        
        ]


    
    def __init__(self,img_src, parent=None):
        QtGui.QMainWindow.__init__(self, parent)
        self.setWindowTitle('Feature Finder')
        
        self.cur_frame = 0
        self.proc_flag = False
        self.refresh_points = False
        self.refresh_img = False
        self.img_src = img_src
        self.create_main_frame()
        self.create_diag()
        self.create_status_bar()
        self.params = dict((d['name'],d['default']) for d in IdenGui.spinner_lst)
        
        self.idenpb = IdenProcessBackend(self.params)
     
        self.points = None
        self.on_draw()
    
    def on_draw(self):
        """ Redraws the figure
        """
        if self.refresh_img:

            self.im.set_data(self.img)
            self.refresh_img = False

        if self.refresh_points:
            self.pt_plt.set_xdata(self.points[0])
            self.pt_plt.set_ydata(self.points[1])
            #            print 'update the points'
            self.refresh_points = False

        self.canvas.draw()
        self.status_text.setNum(self.cur_frame)


    def proc_frame(self):
        if self.proc_flag:
            if self.refresh_img:
                self.img = self.img_src[self.cur_frame]
            self.points = self.idenpb.process_single_frame(self.img)

            self.refresh_points = True
            
        self.prog_bar.hide()            
        self.on_draw()
        
    def set_cur_frame(self,i):
        print 'set_cur_frame'
        if self.cur_frame != i:
            self.refresh_img = True
        self.cur_frame = i
        self.prog_bar.show()
        QtCore.QTimer.singleShot(1,self.proc_frame)



    def create_main_frame(self):
        self.main_frame = QtGui.QWidget()
        # create the mpl Figure and FigCanvas objects. 
        # 5x4 inches, 100 dots-per-inch
        #

        self.fig = Figure((24,24))
        self.canvas = FigureCanvas(self.fig)
        self.canvas.setParent(self.main_frame)

        # Since we have only one plot, we can use add_axes 
        # instead of add_subplot, but then the subplot
        # configuration tool in the navigation toolbar wouldn't
        # work.
        #
        self.axes = self.fig.add_subplot(111)
        self.img = np.asarray(self.img_src[self.cur_frame])
        self.im = self.axes.imshow(self.img,cmap='cubehelix')
        self.axes.set_aspect('equal')

        self.pt_plt, = self.axes.plot([],[],linestyle='none',marker='x',color='k')


        
        # Create the navigation toolbar, tied to the canvas
        #
        self.mpl_toolbar = NavigationToolbar(self.canvas, self.main_frame)
        

        #
        # layout with box sizers
        # 
        
        self.ctrl_button = QtGui.QPushButton("Show &Controls")
        self.ctrl_button.clicked.connect(self.show_cntrls)


        
        hbox = QtGui.QHBoxLayout()


        
        for w in [self.ctrl_button ]:
            hbox.addWidget(w)
            hbox.setAlignment(w, QtCore.Qt.AlignVCenter)

            
        vbox = QtGui.QVBoxLayout()
        vbox.addWidget(self.mpl_toolbar)
        vbox.addWidget(self.canvas)
        vbox.addLayout(hbox)
        
        self.main_frame.setLayout(vbox)
        self.setCentralWidget(self.main_frame)


    def create_diag(self):
        
        # make top level stuff
        self.diag = QtGui.QDialog(parent=self)
        diag_layout = QtGui.QVBoxLayout()
        self.diag.setLayout(diag_layout)
        

        # frame number lives on top
        self.frame_spinner = QtGui.QSpinBox()
        self.frame_spinner.setRange(0,len(self.img_src)-1)
        self.frame_spinner.valueChanged.connect(self.set_cur_frame)
        fs_form = QtGui.QFormLayout()
        fs_form.addRow(QtGui.QLabel('frame #'),self.frame_spinner)


        diag_layout.addLayout(fs_form)
        

        # section for dealing with fringe finding 
        
        # fill the spinners
        for spin_prams in IdenGui.spinner_lst:

            s_type = np.dtype(spin_prams['type']).kind

            if s_type == 'i':
                spin_box = QtGui.QSpinBox(parent=self)
            elif s_type== 'f':
                spin_box = QtGui.QDoubleSpinBox(parent=self)
                spin_box.setDecimals(spin_prams['prec'])
            else:
                print s_type
                continue
            
            spin_box.setRange(spin_prams['min'],spin_prams['max'])
            spin_box.setSingleStep(spin_prams['step'])
            spin_box.setValue(spin_prams['default'])
            name = spin_prams['name']


            spin_box.valueChanged.connect(self._gen_update_closure(name))
            fs_form.addRow(QtGui.QLabel(spin_prams['name']),spin_box)


        
        self.proc_flag_button = QtGui.QCheckBox('process frame')
        self.proc_flag_button.stateChanged.connect(self.set_proc_flag)
        diag_layout.addWidget(self.proc_flag_button)

        # button to grab initial spline
        # grab_button = QtGui.QPushButton('Grab Spline')
        # grab_button.clicked.connect(self.grab_sf_curve)
        # fc_vboxes.addWidget(grab_button)
        # # button to process this frame
        
    def create_status_bar(self):
        self.status_text = QtGui.QLabel(str(self.cur_frame))
        self.prog_bar = QtGui.QProgressBar()
        self.prog_bar.setRange(0,0)
        self.prog_bar.hide()
        self.statusBar().addWidget(self.status_text, 1)
        self.statusBar().addWidget(self.prog_bar, 1)

    def show_cntrls(self):
        self.diag.show()

    def set_proc_flag(self,i):
        self.proc_flag = bool(i)
        self.proc_frame()
        
        self.on_draw()


    def _gen_update_closure(self,name):
        return lambda x :self.update_param(name,x)

    def update_param(self,name,x):
        print name,x
            
        self.idenpb.update_param(name,x)
        self.prog_bar.show()

        QtCore.QTimer.singleShot(1,self.proc_frame)


        
