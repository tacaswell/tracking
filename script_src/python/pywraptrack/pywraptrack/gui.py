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
import matplotlib.gridspec as gridspec

import numpy as np
from .swig_wrappers import *
from .img import *

function = type(lambda x:x)
class idenWorker(QtCore.QObject):
    frame_proced = QtCore.Signal(bool,bool)
    img_src_updated_sig = QtCore.Signal(str)
    
    def __init__(self,params,filter_params,img_src,parent=None):
        QtCore.QObject.__init__(self,parent)

        self.img_src = img_src
        self.idenpb = IdenProcessBackend(params)
        self.filter_params = filter_params
        self.img = None
        self.points = None
        self.res_dict = None


        
        def filter_fun(res_dict, f_params):
            if f_params is None:
                f_params = {}
            
            if res_dict is None:
                return None
            
            index = np.ones(res_dict['x'].shape,dtype=bool)

            if 'I' in res_dict and 'Imax' in f_params:
                I_indx = res_dict['I'] < f_params['Imax']
                index *= I_indx
                
            if 'I' in res_dict and 'Imin' in f_params:
                I_indx = res_dict['I'] > f_params['Imax']
                index *= I_indx

            if 'e' in res_dict and 'e_cut' in f_params:
                e_indx = res_dict['e'] < f_params['e_cut']
                index *= e_indx

            if 'dx' in res_dict and 'dy' in res_dict and 'shift_cut' in f_params:
                shift_indx = np.hypot(res_dict['dx'], res_dict['dy']) < f_params['shift_cut']
                index *= shift_indx
                
            if 'r2' in res_dict and 'rg_cut' in f_params:
                r2_indx = res_dict['r2'] < f_params['rg_cut']
                index *= r2_indx


            out_dict = {}
            for key in res_dict:
                out_dict[key] = res_dict[key][index]

            return out_dict

        self.filter_fun = filter_fun
        
    @QtCore.Slot(int,bool)
    def proc_frame(self, i, proc):
        if self.img_src is None:
            return
        
        self.img = self.img_src[i]
        if proc:
            self.res_dict = self.idenpb.process_single_frame(self.img, ('x', 'y', 'dx', 'dy', 'e', 'r2'))
            self.points = self.filter_fun(self.res_dict, self.filter_params)
        else:
            self.res_dict = None
            self.points = None
            
        self.frame_proced.emit(True,True)

    @QtCore.Slot(str)
    def save_parametrs(self,fname):
        self.idenpb.save_params(fname)

    @QtCore.Slot(str,function)
    def set_img_src(self,fname,wrap_fun):
        new_src = wrap_fun(fname)
        if new_src is None:
            self.img = None
            self.img_src = None
            self.img_src_updated_sig.emit('')
            print 'failed to open'
            return
        self.clear()

        
        self.img_src = new_src
        self.img = new_src[0]
        self.img_src_updated_sig.emit(str(new_src))
        
    def get_points(self):
        return self.points

    def update_param(self,key,val):
        self.idenpb.update_param(key,val)
        
    def update_filter_params(self, f_params):
        if f_params is None:
            f_params = {}
        self.filter_params = f_params
        if self.res_dict is not None:
            self.points = self.filter_fun(self.res_dict, self.filter_params)
            self.frame_proced.emit(True,True)

    def get_frame(self):
        return self.img

    def clear(self):
        self.points = None
        self.img = None

    def __len__(self):
        if self.img_src is None:
            return 1
        return len(self.img_src)
    
class IdenGui(QtGui.QMainWindow):
    '''A class for providing a gui interface for playing with iden parameters ''' 

    proc = QtCore.Signal(int,bool)        # signal to worker to process the frame
    save_parametrs_sig = QtCore.Signal(str)         # signal to save the configuration
    redraw_sig = QtCore.Signal(bool,bool) # signal (to be primarily to be connected to self) to re-draw
    set_new_img_src_sig  = QtCore.Signal(str,function) # signal to pass a new image source to the 
    
    
    spinner_lst_compute = [
        {'name':'p_rad',
         'min':0,
         'max':50,
         'step':1,
         'type':np.int,
         'default':4,
         'tooltip':'The radius[pixel] of the mask used in the band pass filter'},
                  
        {'name':'hwhm',
         'min':0,
         'max':50,
         'step':.1,
         'prec':1,
         'type':np.float,
         'default':1.3,
         'tooltip':'The half-width half-max of the Gaussian used for the band pass'},

         
         {'name':'d_rad',
          'min':0,
          'max':50,
          'step':1,
          'type':np.int,
          'default':3,
          'tooltip':'The radius[pixel] of dilation used to locate local maximums'},
          
        {'name':'mask_rad',
         'min':0,
         'max':50,
         'step':1,
         'type':np.int,
         'default':5,
         'tooltip':'The mask radius[pixel] used for computing sub-pixel accuracy'},
         
         
        {'name':'top_cut',
         'min':0,
         'max':1,
         'step':.001,
         'prec':4,
         'type':np.float,
         'default':.001,
         'tooltip':'The fraction of brightest pixels to squash down to the highest non-squashed value'},
         
        {'name':'threshold',
         'min':0,
         'max':100,
         'step':1,
         'prec':1,
         'type':np.float,
         'default':1.0,
         'tooltip':'Pixels below this value (after the band pass which re-scales all pixels to [0-100]) are set to 0'},

         ]
        
    spinner_lst_filter = [
        {'name':'shift_cut',
         'min':0,
         'max':5,
         'step':.1,
         'prec':1,
         'type':np.float,
         'default':1.5,
         'tooltip':'Particles that move more than this in [pixel] are filtered out'},
         
        {'name':'rg_cut',
         'min':0,
         'max':100,
         'step':1,
         'prec':1,
         'type':np.float,
         'default':10,
         'tooltip':'Particles with radius of gyration greater than this are filtered out'},
              
        {'name':'e_cut',
         'min':0,
         'max':1,
         'step':.1,
         'prec':2,
         'type':np.float,
         'default':.5,
         'tooltip':'Particles with eccentricity greater than this will be filtered out'},
         
         
         
         ]
            

    
    def __init__(self,img_src=None, parent=None):
        QtGui.QMainWindow.__init__(self, parent)
        self.setWindowTitle('Feature Finder')

        params = dict((d['name'],d['default']) for d in IdenGui.spinner_lst_compute)
        
                
        self.cur_frame = 0
        self.proc_flag = False
        self.refresh_points = False
        self.refresh_img = False

        self.filter_spinners = {}
        
        self.thread = QtCore.QThread(parent=self)

        self.worker = idenWorker(params,{},img_src,parent=None)
        #        self.worker.proc_frame(self.cur_frame,False)
        self.worker.moveToThread(self.thread)
        self.worker.frame_proced.connect(self.on_draw)        
        self.worker.img_src_updated_sig.connect(self.update_base_image)        
        
        self.proc.connect(self.worker.proc_frame)    
        self.save_parametrs_sig.connect(self.worker.save_parametrs)    
        self.set_new_img_src_sig.connect(self.worker.set_img_src)    
        self.redraw_sig.connect(self.on_draw)
        
        self.create_main_frame()
        self.create_actions()
        self.create_menu_bar()        
        self.create_diag()
        self.create_status_bar()

        self.update_base_image('')

        self.show()
        self.thread.start()
        #        self.thread.exec_()
        
     
        self.points = None





        QtGui.qApp.exec_()

        
    @QtCore.Slot(bool,bool)
    def on_draw(self,refresh_img=True,refresh_points=True):
        """ Redraws the figure
        """

        if refresh_img and self.im is not None:

            img = self.worker.get_frame()

            if img is not None:
                self.im.set_data(img)


        if refresh_points:
            points = self.worker.get_points()
            if points is not None and self.plot_pts:
                x = points['x'] +.5
                y = points['y'] +.5
                self.pt_plt.set_xdata(x)
                self.pt_plt.set_ydata(y)
                self.hmod1_window.update_axes(hist_plot,x,y)
                self.ph.res_dict = points
            else:
                self.pt_plt.set_xdata([])
                self.pt_plt.set_ydata([])


        self.canvas.draw()

        self.prog_bar.hide()                        
        self.diag.setEnabled(True)

        

    def proc_frame(self):
        self.prog_bar.show()                        
        self.diag.setEnabled(False)
        self.proc.emit(self.cur_frame,self.proc_flag)

            
        
    def set_cur_frame(self,i):

        self.cur_frame = i
        self.proc_frame()


    def update_param(self,name,x):

        self.worker.update_param(name,x)
        self.proc_frame()


    def set_proc_flag(self,i):
        self.proc_flag = bool(i)
        self.plot_pts = self.proc_flag
        self.proc_frame()


    def create_main_frame(self):
        self.main_frame = QtGui.QWidget()
        # create the mpl Figure and FigCanvas objects. 
        # 5x4 inches, 100 dots-per-inch
        #

        self.fig = Figure((24,24))
        self.canvas = FigureCanvas(self.fig)
        self.canvas.setParent(self.main_frame)

        self.ph = PickerHandler(self.canvas)
        # Since we have only one plot, we can use add_axes 
        # instead of add_subplot, but then the subplot
        # configuration tool in the navigation toolbar wouldn't
        # work.
        #
        self.axes = self.fig.add_subplot(111)
        self.im = None

        self.pt_plt, = self.axes.plot([],[],
                                      linestyle='none', 
                                      marker='x', 
                                      color='k',
                                      picker=1)


        
        # Create the navigation toolbar, tied to the canvas
        #
        self.mpl_toolbar = NavigationToolbar(self.canvas, self.main_frame)
        

        #
        # layout with box sizers
        # 
        

        
        hbox = QtGui.QHBoxLayout()


        
        for w in [ ]:
            hbox.addWidget(w)
            hbox.setAlignment(w, QtCore.Qt.AlignVCenter)

            
        vbox = QtGui.QVBoxLayout()
        vbox.addWidget(self.mpl_toolbar)
        vbox.addWidget(self.canvas)
        vbox.addLayout(hbox)
        
        self.main_frame.setLayout(vbox)
        self.setCentralWidget(self.main_frame)

        self.hmod1_window = GraphDialog((1,1),self)
        

    def create_diag(self):
        
        # make top level stuff
        self.diag = QtGui.QDockWidget('controls', parent=self)
        self.addDockWidget(QtCore.Qt.LeftDockWidgetArea,self.diag)
        diag_widget = QtGui.QWidget(self.diag)
        self.diag.setWidget(diag_widget)
        diag_layout = QtGui.QVBoxLayout()
        diag_widget.setLayout(diag_layout)
        

        # frame number lives on top
        self.frame_spinner = QtGui.QSpinBox()
        self.frame_spinner.setRange(0,len(self.worker)-1)
        self.frame_spinner.valueChanged.connect(self.set_cur_frame)
        fs_form = QtGui.QFormLayout()
        fs_form.addRow(QtGui.QLabel('frame #'),self.frame_spinner)


        diag_layout.addLayout(fs_form)
        

        # section for dealing with fringe finding 
        compute_group_box = QtGui.QGroupBox("compute parameters")
        compute_fs_layout = QtGui.QFormLayout()
        # fill the spinners
        for spin_prams in IdenGui.spinner_lst_compute:

            s_type = np.dtype(spin_prams['type']).kind

            if s_type == 'i':
                spin_box = QtGui.QSpinBox(parent=self)
            elif s_type== 'f':
                spin_box = QtGui.QDoubleSpinBox(parent=self)
                spin_box.setDecimals(spin_prams['prec'])
            else:
                print s_type
                continue
            spin_box.setKeyboardTracking(False)
            spin_box.setRange(spin_prams['min'],spin_prams['max'])
            spin_box.setSingleStep(spin_prams['step'])
            spin_box.setValue(spin_prams['default'])
            name = spin_prams['name']

            spin_box.valueChanged.connect(self._gen_update_closure(name))
            l_label = QtGui.QLabel(spin_prams['name'])
            if 'tooltip' in spin_prams:
                l_label.setTooltip(spin_prams['tooltip'])
            compute_fs_layout.addRow(l_label,spin_box)

        compute_group_box.setLayout(compute_fs_layout)
        diag_layout.addWidget(compute_group_box)

        ##### set up the filter parameters 
        filter_group_box = QtGui.QGroupBox("filter parameters")
        compute_fs_layout = QtGui.QFormLayout()
        
        for spin_prams in IdenGui.spinner_lst_filter:

            s_type = np.dtype(spin_prams['type']).kind

            if s_type == 'i':
                spin_box = QtGui.QSpinBox(parent=self)
            elif s_type== 'f':
                spin_box = QtGui.QDoubleSpinBox(parent=self)
                spin_box.setDecimals(spin_prams['prec'])
            else:
                print s_type
                continue
            
            spin_box.setKeyboardTracking(False)
            
            spin_box.setRange(spin_prams['min'],spin_prams['max'])
            spin_box.setSingleStep(spin_prams['step'])
            spin_box.setValue(spin_prams['default'])
            name = spin_prams['name']

            self.filter_spinners[name] = spin_box
            spin_box.setEnabled(False)
            
            l_checkbox = QtGui.QCheckBox('enable')
            l_checkbox.stateChanged.connect(spin_box.setEnabled)
            l_checkbox.stateChanged.connect(self.update_filters_acc.trigger)
            spin_box.valueChanged.connect(self.update_filters_acc.trigger)
            l_h_layout = QtGui.QHBoxLayout()
            l_h_layout.addWidget(spin_box)
            l_h_layout.addWidget(l_checkbox)

            compute_fs_layout.addRow(QtGui.QLabel(spin_prams['name']),l_h_layout)

        filter_group_box.setLayout(compute_fs_layout)
        diag_layout.addWidget(filter_group_box)
        diag_layout.addStretch()
        
        self.proc_flag_button = QtGui.QCheckBox('process frame')
        self.proc_flag_button.stateChanged.connect(self.set_proc_flag)
        diag_layout.addWidget(self.proc_flag_button)

        
        self.save_params_button = QtGui.QPushButton('Save Parameters')
        self.save_params_button.clicked.connect(self.save_param_acc.trigger)
        diag_layout.addWidget(self.save_params_button)



        
    def create_status_bar(self):
        self.status_text = QtGui.QLabel('')
        self.prog_bar = QtGui.QProgressBar()
        self.prog_bar.setRange(0,0)
        self.prog_bar.hide()
        self.statusBar().addPermanentWidget(self.status_text)
        self.statusBar().addWidget(self.prog_bar)

    def create_actions(self):
        self.show_cntrl_acc = QtGui.QAction(u'show controls', self)
        self.show_cntrl_acc.triggered.connect(self.show_diag)

        self.save_param_acc = QtGui.QAction(u'&Save Parameters', self)
        self.save_param_acc.triggered.connect(self.save_config)

        self.open_series_acc =  QtGui.QAction(u'Open Se&ries', self)
        self.open_series_acc.triggered.connect(self.open_series)

        
        self.open_stack_acc =  QtGui.QAction(u'Open S&tack', self)
        self.open_stack_acc.triggered.connect(self.open_stack)

        self.update_filters_acc = QtGui.QAction(u'Update Filters',  self)
        self.update_filters_acc.triggered.connect(self.update_filter)

        self.toggle_hist_acc = QtGui.QAction(u'toggle Hist mod1',  self)
        self.toggle_hist_acc.setCheckable(True)
        self.toggle_hist_acc.toggled.connect(self.hmod1_window.setVisible)
        self.toggle_hist_acc.setChecked(True)
        
    def create_menu_bar(self):
        menubar = self.menuBar()
        fileMenu = menubar.addMenu('&File')
        fileMenu.addAction(self.show_cntrl_acc)
        fileMenu.addAction(self.save_param_acc)
        fileMenu.addAction(self.open_stack_acc)
        fileMenu.addAction(self.open_series_acc)

        diagnosticMenue = menubar.addMenu('&Diagnostic')
        diagnosticMenue.addAction(self.toggle_hist_acc)
        
    def show_diag(self):
        self.diag.show()

    def _gen_update_closure(self,name):
        return lambda x :self.update_param(name,x)

    def save_config(self):
        fname,_ = QtGui.QFileDialog.getSaveFileName(self,caption='Save File')
        if len(fname) > 0:
            self.save_parametrs_sig.emit(fname)

    def open_series(self):
        fname,_ = QtGui.QFileDialog.getOpenFileName(self,caption='Select first image')
        if len(fname) == 0:
            return

        self.diag.setEnabled(False)        
        self.prog_bar.show()

        self.set_new_img_src_sig.emit(fname, Series_wrapper.create_wrapper)
        
    def open_stack(self):
        fname,_ = QtGui.QFileDialog.getOpenFileName(self,caption='Select image stack')
        if len(fname) == 0:
            return

        self.diag.setEnabled(False)
        self.prog_bar.show()
        self.set_new_img_src_sig.emit(fname, Stack_wrapper)
        
    def closeEvent(self,ce):
        self.diag.close()
        self.thread.quit()
        self.thread.wait()
        QtGui.QMainWindow.closeEvent(self,ce)

    def destroy(self,dw=True,dsw=True):
        print 'i died!'
        self.thread.quit()
        self.thread.wait()
        QtGui.QMainWindow.destroy(self,dw,dsw)

    @QtCore.Slot(str)
    def update_base_image(self,label):

        self.proc_flag_button.setChecked(False)

        self.frame_spinner.setValue(0)
        

        self.im = None
        self.frame_spinner.setRange(0,len(self.worker)-1)        
        img = self.worker.get_frame()
        if img is None:
            return

        img = np.asarray(img)

        self.im = self.axes.imshow(img,cmap='cubehelix', extent=[0,img.shape[1],0,img.shape[0]],origin='lower', interpolation='nearest')
        self.axes.set_aspect('equal')
        self.status_text.setText(label)
        self.redraw_sig.emit(True,True)

    @QtCore.Slot()
    def update_filter(self):
        filter_params = {}
        for name,box in self.filter_spinners.iteritems():
            if box.isEnabled():
                filter_params[name] = box.value()


        self.worker.update_filter_params(filter_params)


        
class GraphDialog(QtGui.QDialog):
    def __init__(self, grid_size=(1, 1), parent=None):
        QtGui.QDialog.__init__(self, parent)
        
        self.fig = Figure((10, 10))
        self.canvas = FigureCanvas(self.fig)
        self.canvas.setParent(self)
        self.canvas.setSizePolicy(QtGui.QSizePolicy.Expanding,
                                  QtGui. QSizePolicy.Expanding)
        self.gs = gridspec.GridSpec(*grid_size)
        self.axes_list = [self.fig.add_subplot(s) for s in self.gs]
        self.gs.tight_layout(self.fig, rect=[0, 0, 1, 1])
        
    def update_axes(self, fun, *args,**kwargs):
        fun(self.axes_list, *args,**kwargs)
        self.gs.tight_layout(self.fig)
        self.canvas.draw()

    def resizeEvent(self, re):
        QtGui.QDialog.resizeEvent(self, re)
        self.canvas.resize(re.size().width(), re.size().height())
        self.gs.tight_layout(self.fig, rect=[0, 0, 1, 1],pad=0)

def hist_plot(axes_lst,x,y):
    ax = axes_lst[0]
    ax.cla()
    ax.axhline(1)
    ax.hist(np.mod(x,1),100,histtype='step',normed=True)
    ax.hist(np.mod(y,1),100,histtype='step',normed=True)
    ax.set_ylim(.7,1.3)


    
class PickerHandler(object):
    def __init__(self,canv,fun = None):
        canv.mpl_connect('pick_event',self.on_pick)
        self.fun = fun
        self.res_dict = None
        
    def on_pick(self,event):
        art = event.artist
        # if not a Line2D, don't want to deal with it
        if not isinstance(art,matplotlib.lines.Line2D):
            return
        if self.res_dict is None:
            return
        
        if len(event.ind):
            for i in event.ind:
                print 'Particle :', i
                for k in self.res_dict:
                    print k,': ', self.res_dict[k][i],' '
                print '======'

