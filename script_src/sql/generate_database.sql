CREATE TABLE dsets (
	dset_key INTEGER PRIMARY KEY ASC AUTOINCREMENT,
        fname TEXT NOT NULL UNIQUE, 
        dtype TEXT NOT NULL,
        ddate TEXT NOT NULL DEFAULT CURRENT_DATE,
        sname TEXT NOT NULL,
	ftype INTEGER NOT NULL,
	mtype INTEGER NOT NULL,
	FOREIGN KEY(ftype) REFERENCES format_types(ftype_key),
	FOREIGN KEY(mtype) REFERENCES md_types(mtype_key)
);

CREATE TABLE func_names(
       func_key INTEGER PRIMARY KEY,
       func_name TEXT NOT NULL UNIQUE
);

CREATE TABLE comps(
	comp_key INTEGER PRIMARY KEY ASC AUTOINCREMENT,
	func_key INTEGER NOT NULL,
	dset_key INTEGER NOT NULL,
	FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),
	FOREIGN KEY(func_key) REFERENCES func_names(func_key)
);


CREATE TABLE format_types(
       ftype_key  INTEGER PRIMARY KEY,
       ftype_name TEXT NOT NULL UNIQUE
);

CREATE TABLE md_types(
       mtype_key  INTEGER PRIMARY KEY,
       mtype_name TEXT NOT NULL UNIQUE
);


CREATE TABLE iden(
	comp_key INTEGER PRIMARY KEY,
	dset_key INTEGER NOT NULL,
	threshold FLOAT NOT NULL,
	hwhm FLOAT NOT NULL,
	p_rad INTEGER NOT NULL,
	d_rad INTEGER NOT NULL,
	mask_rad INTEGER NOT NULL,
        top_cut FLOAT NOT NULL,
	frames_avged INTEGER NOT NULL,
	fout TEXT NOT NULL,
	date TEXT NOT NULL DEFAULT CURRENT_DATE,
	FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),
	FOREIGN KEY(comp_key) REFERENCES comps(comp_key)
);

CREATE TABLE gofr (
       comp_key INTEGER PRIMARY KEY,
       dset_key INTEGER NOT NULL,
       iden_key INTEGER NOT NULL,
       nbins INTEGER NOT NULL,
       max_range FLOAT NOT NULL,
       shift_cut FLOAT NOT NULL,
       rg_cut FLOAT NOT NULL,
       e_cut FLOAT NOT NULL,
       fin TEXT NOT NULL,
       fout TEXT NOT NULL,
       date TEXT NOT NULL DEFAULT CURRENT_DATE,
       FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),
       FOREIGN KEY(comp_key) REFERENCES comps(comp_key),
       FOREIGN KEY(iden_key) REFERENCES iden(comp_key)
);

CREATE TABLE gofr_by_plane(
       comp_key INTEGER PRIMARY KEY,
       dset_key INTEGER NOT NULL,
       iden_key INTEGER NOT NULL,
       nbins INTEGER NOT NULL,
       max_range FLOAT NOT NULL,
       comp_count INTEGER NOT NULL,
       frames_per_comp INTEGER NOT NULL,
       shift_cut FLOAT NOT NULL,
       rg_cut FLOAT NOT NULL,
       e_cut FLOAT NOT NULL,
       fin TEXT NOT NULL,
       fout TEXT NOT NULL,
       date TEXT NOT NULL DEFAULT CURRENT_DATE,
       FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),
       FOREIGN KEY(comp_key) REFERENCES comps(comp_key),
       FOREIGN KEY(iden_key) REFERENCES iden(comp_key)
);

CREATE TABLE tracking (
       comp_key INTEGER PRIMARY KEY,
       iden_key INTEGER NOT NULL,  
       dset_key INTEGER NOT NULL,
       search_range FLOAT NOT NULL,
       shift_cut FLOAT NOT NULL,
       rg_cut FLOAT NOT NULL,
       e_cut FLOAT NOT NULL,
       fin TEXT NOT NULL,
       fout TEXT NOT NULL,
       date TEXT NOT NULL DEFAULT CURRENT_DATE,
       FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),
       FOREIGN KEY(comp_key) REFERENCES comps(comp_key),
       FOREIGN KEY(iden_key) REFERENCES iden(comp_key)
);


CREATE TABLE msd_old (
       comp_key INTEGER PRIMARY KEY,
       iden_key INTEGER NOT NULL,
       dset_key INTEGER NOT NULL,
       search_range FLOAT NOT NULL,
       msd_steps INTEGER NOT NULL,
       min_track_length INTEGER NOT NULL,
       shift_cut FLOAT NOT NULL,
       rg_cut FLOAT NOT NULL,
       e_cut FLOAT NOT NULL,
       fin TEXT NOT NULL,
       fout TEXT NOT NULL,
       date TEXT NOT NULL DEFAULT CURRENT_DATE,
       FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),
       FOREIGN KEY(comp_key) REFERENCES comps(comp_key),
       FOREIGN KEY(iden_key) REFERENCES iden(comp_key)
);

CREATE TABLE msd (
       comp_key INTEGER PRIMARY KEY,
       iden_key INTEGER NOT NULL,
       track_key INTEGER NOT NULL,
       dset_key INTEGER NOT NULL,
       msd_steps INTEGER NOT NULL,
       min_track_length INTEGER NOT NULL,
       fin TEXT NOT NULL,
       fout TEXT NOT NULL,
       date TEXT NOT NULL DEFAULT CURRENT_DATE,
       FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),
       FOREIGN KEY(comp_key) REFERENCES comps(comp_key),
       FOREIGN KEY(track_key) REFERENCES tracking(comp_key)
);

CREATE TABLE trk_stat (
       comp_key INTEGER PRIMARY KEY,
       iden_key INTEGER NOT NULL,
       dset_key INTEGER NOT NULL,
       search_range FLOAT NOT NULL,
       steps INTEGER NOT NULL,
       hist_bins INTEGER NOT NULL,
       hist_range FLOAT NOT NULL,
       shift_cut FLOAT NOT NULL,
       rg_cut FLOAT NOT NULL,
       e_cut FLOAT NOT NULL,
       fin TEXT NOT NULL,
       fout TEXT NOT NULL,
       date TEXT NOT NULL DEFAULT CURRENT_DATE,
       FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),
       FOREIGN KEY(comp_key) REFERENCES comps(comp_key),
       FOREIGN KEY(iden_key) REFERENCES iden(comp_key)
);


CREATE TABLE trk_stat_trk (
       comp_key INTEGER PRIMARY KEY,
       trk_key INTEGER NOT NULL,
       dset_key INTEGER NOT NULL,
       steps INTEGER NOT NULL,
       hist_binsINTEGER NOT NULL,
       hist_range FLOAT NOT NULL,
       fin TEXT NOT NULL,
       fout TEXT NOT NULL,
       date TEXT NOT NULL DEFAULT CURRENT_DATE,
       FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),
       FOREIGN KEY(comp_key) REFERENCES comps(comp_key),
       FOREIGN KEY(trk_key) REFERENCES tracking(comp_key)
);

CREATE TABLE vanHove (
       comp_key INTEGER PRIMARY KEY,
       dset_key INTEGER NOT NULL,
       track_key INTEGER NOT NULL,
       min_track_length INTEGER NOT NULL,
       max_step INTEGER NOT NULL,
       max_range FLOAT NOT NULL,
       nbins INTEGER NOT NULL,
       fin TEXT NOT NULL,
       fout TEXT NOT NULL,
       date TEXT NOT NULL DEFAULT CURRENT_DATE,
       FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),
       FOREIGN KEY(comp_key) REFERENCES comps(comp_key),
       FOREIGN KEY(track_key) REFERENCES tracking(comp_key)
);
	
CREATE TABLE phi6 (
       comp_key INTEGER PRIMARY KEY,
       iden_key INTEGER NOT NULL,
       dset_key INTEGER NOT NULL,
       neighbor_range FLOAT NOT NULL,
       shift_cut FLOAT NOT NULL,
       rg_cut FLOAT NOT NULL,
       e_cut FLOAT NOT NULL,
       fin TEXT NOT NULL,
       fout TEXT NOT NULL,
       date TEXT NOT NULL DEFAULT CURRENT_DATE,
       FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),
       FOREIGN KEY(comp_key) REFERENCES comps(comp_key),
       FOREIGN KEY(iden_key) REFERENCES iden(comp_key)       
);

CREATE TABLE data_collection (
       coll_key INTEGER PRIMARY KEY,
       pub_flag INTEGER,
       long_desc TEXT,
       short_desc TEXT NOT NULL
);

CREATE TABLE data_collection_map (
       coll_key INTEGER NOT NULL,
       dset_key INTEGER NOT NULL,
       FOREIGN KEY(dset_key) REFERENCES dsets(dset_key),
       FOREIGN KEY(coll_key) REFERENCES data_collection(coll_key)
);

-- fill in fixed table values

-- format names
insert into format_types (ftype_key,ftype_name) values (1,'stack');
insert into format_types (ftype_key,ftype_name) values (2,'series');
-- md formates
insert into md_types (mtype_key,mtype_name) values (0,'None');
insert into md_types (mtype_key,mtype_name) values (1,'Meta Morph');

-- function names
insert into func_names (func_key,func_name) values  (1, 'Iden');
insert into func_names (func_key,func_name) values  (2, 'Iden_avg');
insert into func_names (func_key,func_name) values  (3, 'gofr');
insert into func_names (func_key,func_name) values  (4, 'gofr3D');
insert into func_names (func_key,func_name) values  (5, 'gofr_by_plane');
insert into func_names (func_key,func_name) values  (6, 'link3D');
insert into func_names (func_key,func_name) values  (7, 'msd');
insert into func_names (func_key,func_name) values  (8, 'phi6');
insert into func_names (func_key,func_name) values  (9, 'track_stats');
insert into func_names (func_key,func_name) values  (10, 'tracking');
insert into func_names (func_key,func_name) values  (11, 'vanHove');
