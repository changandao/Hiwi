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
#include <sstream>

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


	
using namespace std;	


static uiCmdAccessState TestAccessDefault(uiCmdAccessMode access_mode)
{
	return (ACCESS_AVAILABLE);
}


 extern "C" int user_initialize( int argc, char *argv[], char *proe_vsn, char *build, wchar_t err_buff[] )
{
	std::ofstream log("logMain.txt",std::ofstream::app);  //// Log-Datei zur Fehlersuche

	uiCmdCmdId cmd_id;

	ProError error;

	
	////////////////////// Button für Import in Creo

	
	// Festlegen,welche Funktion durch den Button aufgerufen wird
		error = ProCmdActionAdd(
							"Parameter_import", 
							(uiCmdCmdActFn) Parameter_import, 
							uiProe2ndImmediate, 
							TestAccessDefault,
							PRO_B_TRUE, 
							PRO_B_TRUE,
							&cmd_id 
							);
	if(error != 0)
	{
		log << "ProCmdActionAdd for Parameter returned " << error << std::endl;
	}
	
	///// Beschriftung des Buttons
	error = ProCmdDesignate	(
							cmd_id,	
							"import1",
							"import2",	
							"import3",
							L"menu.txt"	
							);
	if(error != 0)
	{
		log << "ProCmdDesignate for Parameter_import returned " << error << std::endl;
	}


	//////////////////////// Button für Export aus Creo
				error = ProCmdActionAdd(
							"Parameter_export", 
							(uiCmdCmdActFn) Parameter_export, 
							uiProe2ndImmediate, 
							TestAccessDefault,
							PRO_B_TRUE, 
							PRO_B_TRUE,
							&cmd_id 
							);
	if(error != 0)
	{
		log << "ProCmdActionAdd for Parameter returned " << error << std::endl;
	}
	error = ProCmdDesignate	(
							cmd_id,	
							"export1",
							"export2",	
							"export3",
							L"menu.txt"	
							);
	if(error != 0)
	{
		log << "ProCmdDesignate for Parameter_export returned " << error << std::endl;
	}


	return 0;
	
}

	  

     extern "C" void user_terminate()
  {
     }



/// Funtion zur Änderung der Parameter in Creo
void import(ProName name, double value)
{
	ProError error; 
	std::ofstream log("logMain.txt",std::ofstream::app);
	ProMdl p_model;
	ProModelitem p_model_item; 
	ProParameter param;
	ProParamvalue provalue;

	
	///ProMdl bekommen 
	error = ProMdlCurrentGet(&p_model);
	if (error != 0)
	{
	log << "ProMdlCurrentGet returned" << error << std::endl;
	}
	
	///ProMdl wird zu ProModelitem
	error = ProMdlToModelitem(p_model,&p_model_item);
	if (error != 0)
	{
	log << "ProMdlToModelitem returned" << error << std::endl;
	}
	
	/// ProParameter initiieren
	error=	ProParameterInit(&p_model_item,name , &param);
	if (error != 0)
	{
	log << "ProParameterInit returned" << error << std::endl;
	}

	/// ProParaneterValue bekommen
	ProParameterValueGet	(&param, &provalue);
		if (error != 0)
	{
	log << "ProParameterValueGet" << error << std::endl;
	}
	
		
	/// ProParamValue setzen
	error = ProParamvalueSet	(&provalue,&value,PRO_PARAM_DOUBLE);
		if (error != 0)
	{
	log << "ProParamValueSet" << error << std::endl;
	
	}
	/// Wert des Parameters in Creo ändern
	error = ProParameterValueWithUnitsSet	(&param,&provalue,NULL);
	if (error !=0)
	{
	log << "ProParameterValueWithUnitsSet" << error << std::endl;
	}



}
	/// struct, in dem Name und Koordinatenwerte gespeichert werden
	struct Values {   
    XMLCh* Name;  
	double x;
	double y;
	double z;
	double rx;
	double ry;
	double rz;
	};

/// Funktion zur Umwandlung von double zu wchar_t*
const wchar_t* d2wc(double value){

	std::string s= std::to_string(value);
	const wchar_t* a = xercesc::XMLString::transcode(s.c_str());
	return a;

}

/// Funktion zur Umwandlung eines strings zu wchar_t*
const wchar_t* s2wc(string s){
	
	const wchar_t* a = xercesc::XMLString::transcode(s.c_str());
	return a;

}


/// Funktion für den Import von Parametern 
/// wird durch Klicken des Buttons "Parameter_import" ausgelöst
uiCmdCmdActFn Parameter_import(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data)
{

	std::ofstream log("logMain.txt",std::ofstream::app);
	ProName proname;
	ProError error; 
	ProMdl p_model;
	ProMdlType promdltype = ProMdlType::PRO_MDL_PART;

	/////////////////////////////////////////////////	xml auslesen
	xercesc::XMLPlatformUtils::Initialize();
	const char* xmlFile = "..\\..\\files\\IM.aml" ;
	XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(gLS);
    DOMLSParser* parser = impl->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	DOMDocument* doc = NULL;
	doc = parser->parseURI(xmlFile);
	DOMElement* root = doc->getDocumentElement();//rootElement
	//DOMElement* rootModel;
	string name = XMLString::transcode(root->getNodeName());

	log << "Root Name: " << name << endl;

	/// struct für das Speichern der Werte aus der xml-Datei
	struct FRAME {   
    string Name; 
    double x; 
	double y; 
	double z; 
	double rx; 
	double ry; 
	double rz; 
	} frame;  
	FRAME child_frame;
	//string parts[] = {"0", "Kran", "Montageplatte", "OptischerSensor", "Werkstückrückgabe"};
	double value;
	//{"Kran", "Montageplatte", "OptischerSensor", "Werkstückrückgabe"};
	string parts[] = {"0", "Kran", "Montageplatte", "OptischerSensor", "Werkstückrückgabe"};

//	double axisNumber[6][5];
	//double x[5],y[5],z[5],rx[5],ry[5],rz[5];
	DOMText* pTextNode = NULL;
	/*DOMText* pTextNode1 = NULL;
	DOMText* pTextNode2 = NULL;
	DOMText* pTextNode3 = NULL;
	DOMText* pTextNode4 = NULL;
	DOMText* pTextNode5 = NULL;
	DOMText* pTextNode6 = NULL;*/
	DOMElement * pChild = NULL;	
	DOMElement * pAttr1 = NULL;
	DOMElement * pAttr2 = NULL;
	DOMElement * pValue = NULL;
	DOMDocument *pDOMDocument = NULL;
	string axises[] = {"x","y","z","rz","ry","rz",}; 

	/// Vektor vom Typ Frame in dem alle Werte gespeichert werden
    std::vector<FRAME> v;
	DOMElement * pInstanceElement = root->getFirstElementChild();
	string instanceName = XMLString::transcode(pInstanceElement->getAttribute(L"Name"));
	parts[0] = instanceName;
	log<<"InstanceName is"<< parts[0]<<endl;
	for(DOMElement* pChild = pInstanceElement->getFirstElementChild(); pChild!=0; pChild = pChild->getNextElementSibling())
	{
		string attr_part = XMLString::transcode(pChild->getAttribute(L"Name"));
		//frame.Name = attr_part;
		for(DOMElement* pChild_part = pChild->getFirstElementChild(); pChild_part!=0; pChild_part = pChild_part->getNextElementSibling())
		{
			string name_child_part = XMLString::transcode(pChild_part->getTagName());
			//log << "Child of Part Name: " << name_child_part << endl;

			/*if (name_child_part.compare("InternalElement")==0)
			{
				attr_part = XMLString::transcode(pChild->getAttribute(L"Name"));
				child_frame.Name = attr_part;
			}else*/ 
			if(name_child_part.compare("Attribute")==0)
			{
				child_frame.Name = attr_part;
				for(DOMElement * pAttr = pChild_part->getFirstElementChild(); pAttr!=0; pAttr=pAttr->getNextElementSibling())
				{
					//name = XMLString::transcode(pAttr->getNodeName());
					attr_part = XMLString::transcode(pAttr->getAttribute(L"Name"));
					DOMElement * pValue = pAttr->getFirstElementChild();
					string text = XMLString::transcode(pValue->getTextContent());
					const char * c = text.c_str();
					value = atof(c);

					if (attr_part.compare("x") == 0 )
					{
						child_frame.x = value;
					}
					else if (attr_part.compare("y") == 0 )
					{
						child_frame.y = value;
					}
					else if (attr_part.compare("z") == 0 )
					{
						child_frame.z = value;
					}
					else if (attr_part.compare("rx") == 0 )
					{
						child_frame.rx = value;
					}
					else if (attr_part.compare("ry") == 0 )
					{
						child_frame.ry = value;
					}
					else if (attr_part.compare("rz") == 0 )
					{
						child_frame.rz = value;
						v.push_back(child_frame);
					}
				}
			}
		}
	}

	log<<"the first test"<< endl;

	ProMdlType p_type = ProMdlType::PRO_MDL_ASSEMBLY;
	string str = parts[0];
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	ProStringToWstring(proname,cstr);

	error = ProMdlCurrentGet(&p_model);
	if (error != 0)
	{
	log << "ProMdlCurrentGet returned" << error << std::endl;
	}
	/*error = ProMdlInit(proname,p_type, &p_model);

	if (error != 0)
	{
	log << "ProMdlInit returned" << error << std::endl;
	}*/

	ProMdlnameShortdata *depd = NULL;
	ProMdlfileType * files = NULL;
	int num_pdep = 0;
	error = ProMdlDependenciesDataList(p_model, &depd, &files, &num_pdep);
	wchar_t test_new_name[] = L"test111_User";

	//ProMdl new_p_model;
	error = ProMdlnameRename(p_model, test_new_name);


	error = ProMdlnameRetrieve((*(depd+3)).name, *(files+3), &p_model);

	
	
	wchar_t test1_new_name[] = L"test_motage11_User";
	log << "I just wanna see what is array name: "<<test1_new_name<< " and this "<< test1_new_name[2]<< " and that: "<< *(test1_new_name+2)<<endl;

	error = ProMdlnameRename(p_model, test1_new_name);

	int windowId = 0;
	error = ProMdlWindowGet(p_model, &windowId);
	log << "I just wanna see what is window ID: : "<< windowId<<endl;
	error = ProMdlErase(p_model);
	if (error != 0)
	{
	log << "ProMdlErase returned" << error << std::endl;
	}


	//error = ProMdlDependenciesCleanup(p_model);
	error = ProTreetoolRefresh(p_model);
	//error = ProMdlDisplay(p_model);
	if (error != 0)
	{
	log << "ProMdlDisplay returned" << error << std::endl;
	}

	/// Aufrufen der Import-Funktion für die einzelnen Koordinaten
	for (int i=0; i<v.size(); i++)
	{
		log<<"we WANG TO KNOW IF IT what is v name:"<<v[i].Name<< endl;
		if (v[i].Name.compare(parts[1]) == 0)
		{
			log<<"we WANG TO KNOW IF IT ENTERS:"<< endl;
			ProStringToWstring(proname,"kran_frame_x");
			import(proname, v[i].x);
			
			ProStringToWstring(proname,"kran_frame_y");
			import(proname, v[i].y);

			ProStringToWstring(proname,"kran_frame_z");
			import(proname, v[i].z);
			
			ProStringToWstring(proname,"kran_frame_rx");
			import(proname, v[i].rx);

			ProStringToWstring(proname,"kran_frame_ry");
			import(proname, v[i].ry);

			ProStringToWstring(proname,"kran_frame_rz");
			import(proname, v[i].rz);
		}else if (v[i].Name.compare(parts[2]) == 0)
		{
			ProStringToWstring(proname,"Montageplatte_frame_x");
			import(proname, v[i].x);
	
			ProStringToWstring(proname,"Montageplatte_frame_y");
			import(proname, v[i].y);

			ProStringToWstring(proname,"Montageplatte_frame_z");
			import(proname, v[i].z);
			
			ProStringToWstring(proname,"Montageplatte_frame_rx");
			import(proname, v[i].rx);

			ProStringToWstring(proname,"Montageplatte_frame_ry");
			import(proname, v[i].ry);

			ProStringToWstring(proname,"Montageplatte_frame_rz");
			import(proname, v[i].rz);
		}else if (v[i].Name.compare(parts[3]) == 0)
		{
			ProStringToWstring(proname,"OptischerSensor_frame_x");
			import(proname, v[i].x);
	
			ProStringToWstring(proname,"OptischerSensor_frame_y");
			import(proname, v[i].y);

			ProStringToWstring(proname,"OptischerSensor_frame_z");
			import(proname, v[i].z);
			
			ProStringToWstring(proname,"OptischerSensor_frame_rx");
			import(proname, v[i].rx);

			ProStringToWstring(proname,"OptischerSensor_frame_ry");
			import(proname, v[i].ry);

			ProStringToWstring(proname,"OptischerSensor_frame_rz");
			import(proname, v[i].rz);
		}else if (v[i].Name.compare(parts[4]) == 0)
		{
			ProStringToWstring(proname,"ABLAGE_frame_X");
			import(proname, v[i].x);
	
			ProStringToWstring(proname,"ABLAGE_frame_Y");
			import(proname, v[i].y);

			ProStringToWstring(proname,"ABLAGE_frame_Z");
			import(proname, v[i].z);
			
			ProStringToWstring(proname,"ABLAGE_frame_RX");
			import(proname, v[i].rx);

			ProStringToWstring(proname,"ABLAGE_frame_RY");
			import(proname, v[i].ry);

			ProStringToWstring(proname,"ABLAGE_frame_RZ");
			import(proname, v[i].rz);
		}
	}
	
	return 0;

}



///Vektor in dem alle Parameternamen gespeichert werden
struct NAMES {   
	string Name;     
	wchar_t* x; 
	wchar_t* y; 
	wchar_t* z; 
	wchar_t* rx; 
	wchar_t* ry; 
	wchar_t* rz; 
} names;

string match_pattern(wchar_t* text)
{
	string pattern = "[[:alpha:]]*";
	wstring wstr = text;
	string str;
	str.assign(wstr.begin(), wstr.end());
	//g << str;
	
    string submatch;
    regex r(pattern, regex::icase);
	smatch results;
	if(regex_search(str, results, r))
	{
		submatch = results.str();
	}
	else{ submatch = "empty!!!!!!!";
	}
	
    return submatch;
}

string w2string(wchar_t* text)
{
	wstring wstr = text;
	string str;
	str.assign(wstr.begin(), wstr.end());
	return str;
}




vector<string> delRep(vector<string> vecSrc)
{
	vector<string> vecTag;
	size_t num_elem = vecSrc.size();
	string name_asm = vecSrc[0];
	vecTag.push_back(name_asm);
	for(int i = 1; i< num_elem; i++)
	{
		if(name_asm != vecSrc[i])
		{
			name_asm = vecSrc[i];
			vecTag.push_back(name_asm);
		}
	}

	return vecTag;
}


string asm_name = "ASM";
string prt_name = "PRT";

void addTrees(TreeNode **Node, ProMdl p_model, map<ProMdlfileType, string> typeHash)
{
	std::ofstream log("addtrees.log",std::ofstream::app);
	ProError error;
	ProMdlnameShortdata *depd;
	ProMdlfileType * files;
	int num_pdep;
	error = ProMdlDependenciesCleanup(p_model);
	error = ProMdlDependenciesDataList(p_model, &depd, &files, &num_pdep);

	for(int i = 0; i < num_pdep; i++)
	{
		(*Node)->appendChild(new TreeNode(w2string((depd+i)->name), typeHash[*(files+i)]));
		TreeNode *child = (*Node)->getChild(i);
		log << "let's see what exatly happens:==  "<< w2string((depd+i)->name)<<endl;
		error = ProMdlnameRetrieve((depd+i)->name, *(files+i), &p_model);
		log << "see what is that  " << typeHash[*(files+i)]<< endl;
		if(typeHash[*(files+i)] == prt_name)
			continue;
		else
			//TreeNode * child = (*Node)->getChild(i);
			addTrees(&child, p_model, typeHash);
	}

}

void saveInternalElement(TreeNode* filetree, DOMDocument *pDOMDocument, DOMElement **pInstanceElement, ofstream &log)
{

	DOMElement * pChild = NULL;	
	//vector<TreeNode *> children = filetree->getChild;
	for(int i = 0; i < filetree->childrenNumber(); i++)
	{
		TreeNode *child = filetree->getChild(i);
		pChild = pDOMDocument->createElement(L"InternalElement");
		log<<"creating name attribute: "<< child->getTextContent() << endl;
		pChild->setAttribute(L"Name", s2wc(child->getTextContent()));
		log<<"creating type attribute: "<< child->getType() << endl;
		pChild->setAttribute(L"Type", s2wc(child->getType()));
		(*pInstanceElement)->appendChild(pChild);
		if(child->hasChildren())
			saveInternalElement(child, pDOMDocument, &pChild, log);
		else
			continue;
	}
	//delete pChild;
	
}


void writeAml(TreeNode* filetree, vector<NAMES> v, vector<string> parts, double **axisNumber, int c)
{
	/// Xerces initiieren 
	//XMLPlatformUtils::Initialize();
	ofstream log("writeaml.log", ofstream::app);
	log << "initiating the platform"<< std::endl;

	xercesc::XMLPlatformUtils::Initialize();
	XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
    DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(gLS);

	DOMDocument *pDOMDocument = NULL;
	string axises[] = {"x","y","z","rx","ry","rz",}; // leeres DOMDocument wird erstellt
	//XMLCh axises[6] = {L"x", L"y"}//, L"y", L"z", L"rx", L"ry", L"rz"};

	///DOMDocument erstellen und Rootelement festlegen
	pDOMDocument = impl->createDocument(0, L"CAEXFile", 0);	// Frame-Element erstellen
	log << "create the root"<< std::endl;
	DOMElement * pRootElement = NULL;		// Frame als Rootelement festlegen
	pRootElement = pDOMDocument->getDocumentElement();
	pRootElement->setAttribute(L"FileName", L"xPPUUC1_1.aml");
	pRootElement->setAttribute(L"SchemaVersion", L"2.15");
	
	log << "create the InstanceHierarchy "<< std::endl;
	DOMElement * pInstanceElement = NULL;	
	XMLCh *InstanceName = XMLString::transcode(parts[0].c_str());
	pInstanceElement = pDOMDocument->createElement(L"InstanceHierarchy");
	pInstanceElement->setAttribute(L"Name", InstanceName);
	XMLString::release(&InstanceName);
	pRootElement->appendChild(pInstanceElement);

	log << "create the InternalElement "<< std::endl;
	saveInternalElement(filetree, pDOMDocument, &pInstanceElement,log);
		///  Modulknoten erstellen					
	/*DOMText* pTextNode = NULL;
	DOMElement * pChild = NULL;	
	DOMElement * pAttr1 = NULL;
	DOMElement * pAttr2 = NULL;
	DOMElement * pValue = NULL;
	for(int i=0; i<c; i++)
		{
			
		
		pChild = pDOMDocument->createElement(L"InternalElement");
		pChild->setAttribute(L"Name", s2wc(v[i].Name));
			pAttr1 = pDOMDocument->createElement(L"Attribute");
			pAttr1->setAttribute(L"Name", L"Frame");
			for(int j=0; j<6; j++){
				pAttr2 = pDOMDocument->createElement(L"Attribute");
				pAttr2->setAttribute(L"AttributeDataType", L"xs:double");
				pAttr2->setAttribute(L"Name", s2wc(axises[j]));
					pValue = pDOMDocument->createElement(L"Value");
					pTextNode=pDOMDocument->createTextNode(d2wc(axisNumber[j][i]));
					pValue->appendChild(pTextNode);
				pAttr2->appendChild(pValue);
				pAttr1->appendChild(pAttr2);
			}
			pChild->appendChild(pAttr1);
		pInstanceElement->appendChild(pChild);
		
	}*/

		/// Kinder der Module erstellen und Werte einfügen
		
	
	/// xml datei schreiben

	DOMLSSerializer *writer = ((DOMImplementationLS*)impl)->createLSSerializer();
	DOMLSOutput *theOutputDesc = ((DOMImplementationLS*)impl)->createLSOutput();
	XMLFormatTarget *formTarget = new LocalFileFormatTarget((XMLCh*)L"..\\..\\files\\newexport.aml");
	cout<<"E:\\cad_Li\\files\\newexport.xml"<<endl;

	theOutputDesc->setByteStream(formTarget);
	writer->getDomConfig()->setParameter(L"format-pretty-print", true);
	writer->write(pDOMDocument, theOutputDesc);
}

//ProError ProUtilNonCsysFeatCheck(
//    ProFeature *feature,
//    ProError	status,
//    ProAppData  num_csys)	
//{
//    ProError err;
//    ProFeattype type;
//
//    err = ProFeatureTypeGet(feature, &type);
//    /*TEST_CALL_REPORT("ProFeatureTypeGet()",
//		    "ProUtilNonCsysFeatCheck()", err, err != PRO_TK_NO_ERROR);*/
//    if (type == PRO_FEAT_CSYS)
//    {
//	((int*)num_csys)[0]++;
//	return (PRO_TK_NO_ERROR);
//    }
//    else
//    {
//	return (PRO_TK_E_FOUND);
//    }
//}

/// Funktion für den Export der Dateien
uiCmdCmdActFn Parameter_export(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data)
{

	ProError error; 
	std::ofstream log("logMain.log",std::ofstream::app);
	ProMdl p_model;
	ProModelitem p_model_item; 
	ProParameter param;
	ProParamvalue provalue;
	ProParamvalueType protype;
	ProMdlType promdltype = ProMdlType::PRO_MDL_ASSEMBLY; //= PRO_ASSEMBLY;
	string Type;
	map<ProMdlfileType, string> typeHash;

	typeHash[ProMdlfileType::PRO_MDLFILE_ASSEMBLY] = asm_name;
	typeHash[ProMdlfileType::PRO_MDLFILE_PART] = prt_name;
	
//	int p_model_num;
//	double value;
	vector<Values> values;
	vector<string> parts;
	ProMdlName test_name;
//	ProMdlType test_type;

	///Export the model
	error = ProMdlCurrentGet(&p_model);
	if (error != 0)
	{
	log << "ProMdlCurrentGet returned" << error << std::endl;
	}
	
	error = ProMdlMdlnameGet(p_model, test_name);
	parts.push_back(match_pattern(test_name));
	TreeNode *mdltree =new TreeNode(w2string(test_name), typeHash[ProMdlfileType::PRO_MDLFILE_ASSEMBLY]);

	
	ProMdlnameShortdata *depd = NULL;
	ProMdlfileType * files = NULL;
	int num_pdep = 0;
	error = ProMdlDependenciesDataList(p_model, &depd, &files, &num_pdep);

	int n_csys = 0;

	/*error = ProSolidFeatVisit((ProSolid)p_model, 
	(ProFeatureVisitAction)ProUtilNonCsysFeatCheck, 
	(ProFeatureFilterAction)NULL, (ProAppData)&n_csys);*/
	error = ProMdlToModelitem(p_model,&p_model_item);
	
	if (error != 0)
	{
	log << "ProSessionMdlList returned" << error << std::endl;
	}
	log<<"the third test"<< endl;

	string bauObject;
	string delimiter = "[[:alpha:]]*";
	for(int i = 0; i<num_pdep; i++)
	{
		wcscpy(test_name, (*(depd+i)).name);
		//error = ProMdlMdlnameGet(*(p_model_array+i), );
		
		bauObject = match_pattern(test_name);
		parts.push_back(bauObject);
		//log<< "the name is  " << bauObject << std::endl;

	}

	//ProWstringFree(test_name);


	parts = delRep(parts);
    log<< "the name1 is  " << parts[0] << "  " << parts[1] <<parts[2]<< std::endl;


	vector<NAMES> v;
	int c = 4;


	names.Name	= parts[2];
	names.x		= L"Kran_frame_x";
	names.y		= L"kran_frame_y";
	names.z		= L"kran_frame_z";
	names.rx	= L"kran_frame_rx";
	names.ry	= L"kran_frame_ry";
	names.rz	= L"kran_frame_rz";

	v.push_back(names);


	names.Name	= parts[1];
	names.x		= L"Montageplatte_frame_x";
	names.y		= L"Montageplatte_frame_y";
	names.z		= L"Montageplatte_frame_z";
	names.rx	= L"Montageplatte_frame_rx";
	names.ry	= L"Montageplatte_frame_ry";
	names.rz	= L"Montageplatte_frame_rz";

	v.push_back(names);

	names.Name	= parts[4];
	names.x		= L"OptischerSensor_frame_x";
	names.y		= L"OptischerSensor_frame_y";
	names.z		= L"OptischerSensor_frame_z";
	names.rx	= L"OptischerSensor_frame_rx";
	names.ry	= L"OptischerSensor_frame_ry";
	names.rz	= L"OptischerSensor_frame_rz";

	v.push_back(names);


	names.Name	=parts[3];
	names.x		= L"ABLAGE_frame_X";
	names.y		= L"ABLAGE_frame_Y";
	names.z		= L"ABLAGE_frame_Z";
	names.rx	= L"ABLAGE_frame_RX";
	names.ry	= L"ABLAGE_frame_RY";
	names.rz	= L"ABLAGE_frame_RZ";

	v.push_back(names);


	/*names.Name	= "xPPU_UseCase1";
	names.x		= L"usecase_x";
	names.y		= L"usecase_y";
	names.z		= L"usecase_z";
	names.rx	= L"usecase_rx";
	names.ry	= L"usecase_ry";
	names.rz	= L"usecase_rz";

	v.push_back(names);*/

	//error = ProMdlMdlnameGet(*(p_model_array+p_model_num-1), test_name);
   
	//ProSolidFeatVisit
	//ProAsmcomppathInit
	
	if (error != 0)
	{
	log << "ProMdlToModelitem returned" << error << std::endl;
	}
	int num_axis = 6;

	double **axisNumber;  
	axisNumber = new double *[num_axis];  
	for(int i=0;i<num_axis;i++)  
	{  
		axisNumber[i]=new double [c];  
		memset(axisNumber[i],0,c*sizeof(double)); //attendtion hier ist array[i]  
	}  


	/// Parameterwerte werden eingelesen, in arrays gespeichert und in xml-Datei füllen
	for(int i=0;i<4;i++)
	{
		/// x
		error=	ProParameterInit(&p_model_item,v[i].x , &param);
		if (error != 0)
		{
			log << "ProParameterInit returned" << error << std::endl;
		}


		ProParameterValueGet	(&param, &provalue);
		if (error != 0)
		{
			log << "ProParameterValueGet" << error << std::endl;
		}


		ProParamvalueTypeGet	(&provalue, &protype);
		if (error != 0)
		{
			log << "ProParamvalueTypeGet" << error << std::endl;
		}

		ProParamvalueValueGet	(&provalue, protype, &axisNumber[0][i]);
		if (error != 0)
		{
			log << "ProParamvalueValueGet" << error << std::endl;
		}

		log <<" processing the bauelement: " << w2string(v[i].x) << std::endl;
		///y
		error=	ProParameterInit(&p_model_item,v[i].y , &param);
		if (error != 0)
		{
			log << "ProParameterInit returned" << error << std::endl;
		}


		ProParameterValueGet	(&param, &provalue);
		if (error != 0)
		{
			log << "ProParameterValueGet" << error << std::endl;
		}


		ProParamvalueTypeGet	(&provalue, &protype);
		if (error != 0)
		{
			log << "ProParamvalueTypeGet" << error << std::endl;
		}

		ProParamvalueValueGet	(&provalue, protype, &axisNumber[1][i]);
		if (error != 0)
		{
			log << "ProParamvalueValueGet" << error << std::endl;
		}
		log <<" processing the bauelement: " << v[i].y << std::endl;


		///z
		error=	ProParameterInit(&p_model_item,v[i].z , &param);
		if (error != 0)
		{
			log << "ProParameterInit returned" << error << std::endl;
		}


		ProParameterValueGet	(&param, &provalue);
		if (error != 0)
		{
			log << "ProParameterValueGet" << error << std::endl;
		}


		ProParamvalueTypeGet	(&provalue, &protype);
		if (error != 0)
		{
			log << "ProParamvalueTypeGet" << error << std::endl;
		}

		ProParamvalueValueGet	(&provalue, protype, &axisNumber[2][i]);
		if (error != 0)
		{
			log << "ProParamvalueValueGet" << error << std::endl;
		}
		log <<" processing the bauelement: " << v[i].z << std::endl;


		///rx
		error=	ProParameterInit(&p_model_item,v[i].rx , &param);
		if (error != 0)
		{
			log << "ProParameterInit returned" << error << std::endl;
		}


		ProParameterValueGet	(&param, &provalue);
		if (error != 0)
		{
			log << "ProParameterValueGet" << error << std::endl;
		}


		ProParamvalueTypeGet	(&provalue, &protype);
		if (error != 0)
		{
			log << "ProParamvalueTypeGet" << error << std::endl;
		}

		ProParamvalueValueGet	(&provalue, protype, &axisNumber[3][i]);
		if (error != 0)
		{
			log << "ProParamvalueValueGet" << error << std::endl;
		}
		log <<" processing the bauelement: " << v[i].rx << std::endl;


		///ry
		error=	ProParameterInit(&p_model_item,v[i].ry , &param);
		if (error != 0)
		{
			log << "ProParameterInit returned" << error << std::endl;
		}


		ProParameterValueGet	(&param, &provalue);
		if (error != 0)
		{
			log << "ProParameterValueGet" << error << std::endl;
		}


		ProParamvalueTypeGet	(&provalue, &protype);
		if (error != 0)
		{
			log << "ProParamvalueTypeGet" << error << std::endl;
		}

		ProParamvalueValueGet	(&provalue, protype, &axisNumber[4][i]);
		if (error != 0)
		{
			log << "ProParamvalueValueGet" << error << std::endl;
		}
		log <<" processing the bauelement: " << v[i].ry << std::endl;


		///rz
		error=	ProParameterInit(&p_model_item,v[i].rz , &param);
		if (error != 0)
		{
			log << "ProParameterInit returned" << error << std::endl;
		}


		ProParameterValueGet	(&param, &provalue);
		if (error != 0)
		{
			log << "ProParameterValueGet" << error << std::endl;
		}


		ProParamvalueTypeGet	(&provalue, &protype);
		if (error != 0)
		{
			log << "ProParamvalueTypeGet" << error << std::endl;
		}


		ProParamvalueValueGet	(&provalue, protype, &axisNumber[5][i]);
		if (error != 0)
		{
			log << "ProParamvalueValueGet" << error << std::endl;
		}
		log <<" processing the bauelement: " << v[i].rz << std::endl;


	}

	///DOM
	addTrees(&mdltree, p_model, typeHash);
	log<< "let's see what's in the trees:  "<< mdltree->getChild(1)->getChild(0)->getTextContent()<<
		" and "<< mdltree->getChild(2)->childrenNumber()<< " and "<< mdltree->getChild(3)->getTextContent() <<endl;

	log<<"the fifth test"<< endl;
	
	/*log<<" what does a path mean ? " << w2string((depd)->name) <<"+=+++"<< num_pdep <<std::endl;
	string result_type = w2string((depd)->type);
	log<<"This is just a test: "<< result_type << " IF it's a char?"<<std::endl;*/
	
	writeAml(mdltree, v, parts, axisNumber, c);

	///// Xerces initiieren 
	//XMLPlatformUtils::Initialize();
	//xercesc::XMLPlatformUtils::Initialize();
	//XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
 //   DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(gLS);

	//DOMText* pTextNode = NULL;
	//DOMElement * pChild = NULL;	
	//DOMElement * pAttr1 = NULL;
	//DOMElement * pAttr2 = NULL;
	//DOMElement * pValue = NULL;
	//DOMDocument *pDOMDocument = NULL;
	//string axises[] = {"x","y","z","rx","ry","rz",}; // leeres DOMDocument wird erstellt
	////XMLCh axises[6] = {L"x", L"y"}//, L"y", L"z", L"rx", L"ry", L"rz"};

	/////DOMDocument erstellen und Rootelement festlegen
	//pDOMDocument = impl->createDocument(0, L"CAEXFile", 0);							// Frame-Element erstellen
	//log << "create the root"<< std::endl;
	//DOMElement * pRootElement = NULL;															// Frame als Rootelement festlegen
	//pRootElement = pDOMDocument->getDocumentElement();
	//pRootElement->setAttribute(L"FileName", L"xPPUUC1_1.aml");
	//pRootElement->setAttribute(L"SchemaVersion", L"2.15");
	////string p_model_name = p_model_array[1];
	///*pRootElement->setAttribute(L"Test", (*depd).name);
	//pRootElement->setAttribute(L"Test1", (*(depd+1)).name);
	//pRootElement->setAttribute(L"Test2", (*(depd+2)).name);
	//pRootElement->setAttribute(L"Test3", (*(depd+3)).name);*/
	//log << "create the InstanceHierarchy "<< std::endl;
	//DOMElement * pInstanceElement = NULL;	
	//XMLCh *InstanceName = XMLString::transcode(parts[0].c_str());
	//pInstanceElement = pDOMDocument->createElement(L"InstanceHierarchy");
	//pInstanceElement->setAttribute(L"Name", InstanceName);
	//XMLString::release(&InstanceName);
	//pRootElement->appendChild(pInstanceElement);
	//log << "create the InternalElement "<< std::endl;

	//std::wstring s;
	//std::basic_ostringstream<wchar_t> oss;
	//

	//	///  Modulknoten erstellen												
	//	


	//	/// Kinder der Module erstellen und Werte einfügen
	//	for(int i=0; i<c; i++)
	//	{
	//	
	//	pChild = pDOMDocument->createElement(L"InternalElement");
	//	pChild->setAttribute(L"Name", s2wc(v[i].Name));
	//		pAttr1 = pDOMDocument->createElement(L"Attribute");
	//		pAttr1->setAttribute(L"Name", L"Frame");
	//		for(int j=0; j<6; j++){
	//			pAttr2 = pDOMDocument->createElement(L"Attribute");
	//			pAttr2->setAttribute(L"AttributeDataType", L"xs:double");
	//			pAttr2->setAttribute(L"Name", s2wc(axises[j]));
	//				pValue = pDOMDocument->createElement(L"Value");
	//				pTextNode=pDOMDocument->createTextNode(d2wc(axisNumber[j][i]));
	//				pValue->appendChild(pTextNode);
	//			pAttr2->appendChild(pValue);
	//			pAttr1->appendChild(pAttr2);
	//		}
	//		pChild->appendChild(pAttr1);
	//	pInstanceElement->appendChild(pChild);
	//	
	//}
	//

	//

	///// xml datei schreiben

	//DOMLSSerializer *writer = ((DOMImplementationLS*)impl)->createLSSerializer();
	//DOMLSOutput *theOutputDesc = ((DOMImplementationLS*)impl)->createLSOutput();
	//XMLFormatTarget *formTarget = new LocalFileFormatTarget((XMLCh*)L"..\\..\\files\\export.aml");
	//cout<<"E:\\cad_Li\\files\\export.xml"<<endl;

	//theOutputDesc->setByteStream(formTarget);
	//writer->getDomConfig()->setParameter(L"format-pretty-print",true);
	//writer->write(pDOMDocument, theOutputDesc);

	//error = ProArrayFree(p_model_array);
	//if(error != 0)
	//{
	//	log << "ProArrayFree" << error << std::endl;
	//}
	//delete impl;
	//delete axisNumber;
	////delete p_model_array;
	//delete pTextNode;
	//delete pChild;	
	//delete pAttr1;
	//delete pAttr2;
	//delete pValue;
	//delete pDOMDocument;
	//delete writer;
	//delete theOutputDesc;
	//delete formTarget;
	//delete files;
	//delete depd;
	return 0;
	
}

