#ifndef MAIN_H
#define MAIN_H

/// C++ Bibliotheken
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <map>
#include "TreeNode.h"
#include <algorithm>

///Header für Toolkit
#include <ProToolkit.h>
#include <ProCore.h>
#include <ProSolid.h>
#include <ProFeature.h>
#include <ProMenu.h>
#include <ProEdge.h>
#include <ProToolkitDll.h>
#include <ProMdl.h>
#include <ProMessage.h>
#include <ProModelitem.h>
#include <ProUtil.h>
#include <ProMenuBar.h>
#include <ProUICmd.h>
#include <ProGeomitemdata.h>
#include <ProObjects.h>
#include <ProCurvedata.h>
#include <ProSurfacedata.h>
#include <ProCsysdata.h>
#include <ProParameter.h>
#include <fstream>
#include <ProSelection.h>
#include <ProMdl.h>


/// Header für DOM (Xerces) 
#include <PlatformUtils.hpp>
#include <AbstractDOMParser.hpp>
#include <DOMImplementation.hpp>
#include <DOMImplementationLS.hpp>
#include <DOMImplementationRegistry.hpp>
#include <DOMLSParser.hpp>
#include <DOMException.hpp>
#include <DOMDocument.hpp>
#include <DOMElement.hpp>
#include <DOMCount.hpp>
#include <DOMNodeList.hpp>
#include <DOMError.hpp>
#include <DOMLocator.hpp>
#include <DOMNamedNodeMap.hpp>
#include <DOMAttr.hpp>
#include <DOMErrorHandler.hpp>
#include <XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>


//Funktionsprototypen
static uiCmdAccessState TestAccessDefault(uiCmdAccessMode access_mode);
uiCmdCmdActFn Parameter_import(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data);
uiCmdCmdActFn Parameter_export(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data);
uiCmdCmdActFn Model_import(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data);

using namespace std;	

struct NAMES {   
	string Name;     
	wchar_t* x; 
	wchar_t* y; 
	wchar_t* z; 
	wchar_t* rx; 
	wchar_t* ry; 
	wchar_t* rz; 
} names;

struct Values {   
	XMLCh* Name;  
	double x;
	double y;
	double z;
	double rx;
	double ry;
	double rz;
};

string asm_name = "ASM";
string prt_name = "PRT";

string w2string(wchar_t* text);
//transform wchar_t to string

string match_pattern(wchar_t* text);
//find modle name without symbols like "_" and numbers

vector<string> delRep(vector<string> vecSrc);
//delete repetetive model names

int SequenceSearch(const vector<string> &part_names, string Paraname);
//search certain modle name in the top level models

void addTrees(TreeNode **Node, ProMdl p_model, map<ProMdlfileType, string> typeHash);
//generate model tree

void writeAml(TreeNode* filetree, vector<NAMES> v, vector<string> parts, double **axisNumber, int c);
// write AML file to local disk

void saveInternalElement(TreeNode* filetree, DOMDocument *pDOMDocument, DOMElement **pInstanceElement, ofstream &log,
						 vector<string> parts, double **axisNumber);
//generate DOM trees of all models

#endif
