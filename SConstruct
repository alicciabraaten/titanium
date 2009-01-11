#!/usr/bin/env python

# common SConscripts
import os, re, sys, inspect, os.path as path
import subprocess

filename = inspect.currentframe().f_code.co_filename


OS = ''
os_define = ''

install_prefix = '/usr/local'
product_name = 'Titanium'
global_variable_name = 'ti'

Export('install_prefix')
Export('product_name')
Export('global_variable_name')

class BuildConfig(object): 
	def __init__(self):
		if not hasattr(os, 'uname') or self.matches('CYGWIN'):
			self.os = 'win32'
		elif self.matches('Darwin'):
			self.os = 'osx'
		elif self.matches('Linux'):
			self.os = 'linux'
		self.abstopdir = path.abspath('.')
		self.dir = '#build/%s' % self.os 
		self.krolldir = '#kroll/build/%s' % self.os
		self.absdir = path.abspath('build/%s' % self.os)
		self.krollabsdir = path.abspath('kroll/build/%s' % self.os)
		self.third_party = path.abspath('thirdparty/%s' % self.os)
	def matches(self, n): return bool(re.match(os.uname()[0], n))
	def is_linux(self): return self.os == 'linux'
	def is_osx(self): return self.os == 'osx'
	def is_win32(self): return self.os == 'win32'
	
tiBuild = BuildConfig()

tiBuild.include_dir = path.abspath(path.join('kroll', 'build', 'include'))
tiBuild.env = Environment(
    CPPDEFINES = {
                  'OS_' + tiBuild.os.upper(): 1,
                  '_INSTALL_PREFIX': install_prefix,
                  '_PRODUCT_NAME': product_name,
				  '_GLOBAL_NS_VARNAME' : global_variable_name
                 },
    CPPPATH=['#.', tiBuild.include_dir],
    LIBPATH=[tiBuild.dir, tiBuild.krolldir],
		LIBS=['kroll']
)

#
#if build.env.GetOption('clean'):
#	os.chdir(path.abspath('kroll'))
#	os.system('scons.bat -c PRODUCT_NAME=Titanium')
#	os.chdir(build.abstopdir)
#else:
#	os.chdir(path.abspath('kroll'))
#	os.system('scons.bat PRODUCT_NAME=Titanium')
#	os.chdir(build.abstopdir)
	
# debug build flags
if ARGUMENTS.get('debug', 0) == 1:
	tiBuild.env.Append(CPPDEFINES = {'DEBUG' : 1})
	if not tiBuild.is_win32():
		tiBuild.env.Append(CCFLAGS = ['-g'])
else:
	tiBuild.env.Append(CPPDEFINES = {'NDEBUG' : 1 })
	if not tiBuild.is_win32():
		tiBuild.env.Append(CCFLAGS = ['-O9'])

# turn on special debug printouts for reference counting
if ARGUMENTS.get('debug_refcount', 0) == 1:
	tiBuild.env.Append(CPPDEFINES = {'DEBUG_REFCOUNT': 1})

if tiBuild.is_win32():
	tiBuild.env.Append(CCFLAGS=['/EHsc'])
	tiBuild.env.Append(CPPDEFINES={'WIN32_CONSOLE': 1})
	tiBuild.env.Append(LINKFLAGS=['/DEBUG', '/PDB:${TARGET}.pdb'])

if tiBuild.is_linux() or tiBuild.is_osx():
    tiBuild.env.Append(CPPFLAGS=['-Wall', '-Werror','-fno-common'])

if tiBuild.is_osx():
	OSX_SDK = '/Developer/SDKs/MacOSX10.4u.sdk'
	OSX_UNIV_COMPILER = '-isysroot '+OSX_SDK+' -arch i386'
	OSX_UNIV_LINKER = '-isysroot '+OSX_SDK+' -syslibroot,'+OSX_SDK
	tiBuild.env.Append(CXXFLAGS=OSX_UNIV_COMPILER)
	tiBuild.env.Append(LDFLAGS=OSX_UNIV_LINKER)


Export ('tiBuild')

SConscript('kroll/SConscript')
SConscript('modules/SConscript')
