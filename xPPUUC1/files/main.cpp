
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
#include <vector>



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
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <string>
#include <sstream>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
//#include <xercesc\util\Transcoders\Win32\Win32TransService.hpp>
	

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
	std::ofstream log("logMain.log",std::ofstream::app);

	uiCmdCmdId cmd_id;

	ProError error;

	
	////////////////////// Button für Import

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


	//////////////////////// Button für Export
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












//void DoOutput2File(DOMDocument* pmyDOMDocument, const wchar_t * FullFilePath )
//{
//
//    DOMImplementation    *pImplement        = NULL;
//    DOMWriter            *pSerializer    = NULL;
//    XMLFormatTarget        *pTarget        = NULL;
//
//    /*
//    Return the first registered implementation that 
//    has the desired features. In this case, we are after
//    a DOM implementation that has the LS feature... or Load/Save.
//    */
//    pImplement = DOMImplementationRegistry::getDOMImplementation(L"LS");
//
//    /*
//    From the DOMImplementation, create a DOMWriter.
//    DOMWriters are used to serialize a DOM tree [back] into an XML document.
//    */
//    pSerializer = ((DOMImplementationLS*)pImplement)->createDOMWriter();
//
//
//    /*
//    This line is optional. It just sets a feature 
//    of the Serializer to make the output
//    more human-readable by inserting line-feeds, 
//    without actually inserting any new elements/nodes
//    into the DOM tree. (There are many different features to set.) 
//    Comment it out and see the difference.
//    */
//    pSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
//
//
//    /*
//    Choose a location for the serialized output. The 3 options are:
//        1) StdOutFormatTarget     (std output stream -  good for debugging)
//        2) MemBufFormatTarget     (to Memory)
//        3) LocalFileFormatTarget  (save to file)
//        (Note: You'll need a different header file for each one)
//        Don't forget to escape file-paths with a backslash character, or
//        just specify a file to be saved in the exe directory.
//        
//          eg. c:\\example\\subfolder\\pfile.xml
//
//    */
//    pTarget = new LocalFileFormatTarget(FullFilePath);
//
//
//    // Write the serialized output to the target.
//    pSerializer->writeNode(pTarget, *pmyDOMDocument);
//}


void import(ProName name, double value)
{
	ProError error; 
	std::ofstream log("logMain.log",std::ofstream::app);
	ProMdl p_model;
	ProModelitem p_model_item; 
	ProParameter param;
	ProParamvalue provalue;

			error = ProMdlCurrentGet(&p_model);
	if (error != 0)
	{
	log << "ProMdlCurrentGet returned" << error << std::endl;
	}
	
		error = ProMdlToModelitem(p_model,&p_model_item);
	if (error != 0)
	{
	log << "ProMdlToModelitem returned" << error << std::endl;
	}
	
	

	error=	ProParameterInit(&p_model_item,name , &param);
	if (error != 0)
	{
	log << "ProParameterInit returned" << error << std::endl;
	}


	
	ProParameterValueGet	(&param, &provalue);
		if (error != 0)
	{
	log << "ProParameterValueGet" << error << std::endl;
	}

		
	
	error = ProParamvalueSet	(&provalue,&value,PRO_PARAM_DOUBLE);
		if (error != 0)
	{
	log << "ProParamValueSet" << error << std::endl;
	
	}
	
	error = ProParameterValueWithUnitsSet	(&param,&provalue,NULL);
	if (error !=0)
	{
	log << "ProParameterValueWithUnitsSet" << error << std::endl;
	}



}

	struct Values {   // Declare FRAME struct type  
    XMLCh* Name;   // Declare member types  
	double x;
	double y;
	double z;
	double rx;
	double ry;
	double rz;
	};


void values(){
	

	ProError error; 
	std::ofstream log("logMain.log",std::ofstream::app);
	ProMdl p_model;
	ProModelitem p_model_item; 
	ProParameter param;
	ProParamvalue provalue;
	ProParamvalueType protype;
	double value;
	vector<Values> values;


		XMLPlatformUtils::Initialize();
	xercesc::XMLPlatformUtils::Initialize();
	const char* xmlFile = "E:\\cad_Li\\files\\xml2.xml" ;
	XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
    DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(gLS);

	struct NAMES {   // Declare FRAME struct type  
    string Name;   // Declare member types  
    wchar_t* x; 
	wchar_t* y; 
	wchar_t* z; 
	wchar_t* rx; 
	wchar_t* ry; 
	wchar_t* rz; 
	} names;

	  DOMText* pTextNode = NULL;
  DOMText* pTextNode1 = NULL;
  DOMText* pTextNode2 = NULL;
  DOMText* pTextNode3 = NULL;
  DOMText* pTextNode4 = NULL;
  DOMText* pTextNode5 = NULL;
  DOMText* pTextNode6 = NULL;
DOMElement * pChild = NULL;

		vector<NAMES> v;
		
		Values a[5];

	names.Name	= "Kran";
	names.x		= L"Kran_x";
	names.y		= L"kran_y";
	names.z		= L"kran_z";
	names.rx	= L"kran_rx";
	names.ry	= L"kran_ry";
	names.rz	= L"kran_rz";

	v.push_back(names);


	names.Name	= "Montageplatte";
	names.x		= L"Montageplatte_x";
	names.y		= L"Montageplatte_y";
	names.z		= L"Montageplatte_z";
	names.rx	= L"Montageplatte_rx";
	names.ry	= L"Montageplatte_ry";
	names.rz	= L"Montageplatte_rz";

	v.push_back(names);

	names.Name	= "OptischerSensor";
	names.x		= L"OptischerSensor_x";
	names.y		= L"OptischerSensor_y";
	names.z		= L"OptischerSensor_z";
	names.rx	= L"OptischerSensor_rx";
	names.ry	= L"OptischerSensor_ry";
	names.rz	= L"OptischerSensor_rz";

	v.push_back(names);


	names.Name	= "AblageStapel";
	names.x		= L"ABLAGE_X";
	names.y		= L"ABLAGE_Y";
	names.z		= L"ABLAGE_Z";
	names.rx	= L"ABLAGE_RX";
	names.ry	= L"ABLAGE_RY";
	names.rz	= L"ABLAGE_RZ";

	v.push_back(names);

	names.Name	= "xPPU_UseCase1";
	names.x		= L"usecase_x";
	names.y		= L"usecase_y";
	names.z		= L"usecase_z";
	names.rx	= L"usecase_rx";
	names.ry	= L"usecase_ry";
	names.rz	= L"usecase_rz";

	v.push_back(names);


	
	


	error = ProMdlCurrentGet(&p_model);
	if (error != 0)
	{
	log << "ProMdlCurrentGet returned" << error << std::endl;
	}
	
		error = ProMdlToModelitem(p_model,&p_model_item);
	if (error != 0)
	{
	log << "ProMdlToModelitem returned" << error << std::endl;
	}
	
	for(int i=0; i<5;i++)
	{
	
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

		ProParamvalueValueGet	(&provalue, protype, &value);
		if (error != 0)
		{
		log << "ProParamvalueValueGet" << error << std::endl;
		}
	
		a[i].x = value;
	
		log << "Value" << a[i].x<< std::endl;
		}

		   std::basic_ostringstream<wchar_t> oss;
   oss << value;
   std::wstring s = oss.str();
    const wchar_t* c = s.c_str();

	
   DOMDocument *pDOMDocument = NULL;															// leeres DOMDocument wird erstellt 
log << 2<< std::endl;

pDOMDocument = impl->createDocument(0, L"Frame", 0);							// Frame-Element erstellen
log << 3<< std::endl;
DOMElement * pRootElement = NULL;															// Frame als Rootelement festlegen
pRootElement = pDOMDocument->getDocumentElement();
log << 4<< std::endl;
DOMElement * pModulElement = NULL;															//  Modulknoten erstellen												
pModulElement = pDOMDocument->createElement(L"Modul");
 pRootElement->appendChild(pModulElement);
 log << 5<< std::endl;



pChild = pDOMDocument->createElement(L"Name");
    pTextNode=pDOMDocument->createTextNode(L"Kran");
    pChild->appendChild(pTextNode);
pModulElement->appendChild(pChild);

pChild = pDOMDocument->createElement(L"x");
    pTextNode1=pDOMDocument->createTextNode((XMLCh*)c);
    pChild->appendChild(pTextNode1);
pModulElement->appendChild(pChild);

pChild = pDOMDocument->createElement(L"y");
    pTextNode2=pDOMDocument->createTextNode(L"a");
    pChild->appendChild(pTextNode2);
pModulElement->appendChild(pChild);

pChild = pDOMDocument->createElement(L"z");
    pTextNode3=pDOMDocument->createTextNode(L"b");
    pChild->appendChild(pTextNode3);
pModulElement->appendChild(pChild);

   //std::basic_ostringstream<wchar_t> oss;
   //oss << value;
   //std::wstring s = oss.str();
   //const wchar_t* c = s.c_str();

	

	

DOMLSSerializer *writer = ((DOMImplementationLS*)impl)->createLSSerializer();
DOMLSOutput *theOutputDesc = ((DOMImplementationLS*)impl)->createLSOutput();
XMLFormatTarget *formTarget = new LocalFileFormatTarget((XMLCh*)L"C:\\Users\\prakti2\\Desktop\\export.xml");

theOutputDesc->setByteStream(formTarget);
writer->getDomConfig()->setParameter(L"format-pretty-print",true);
writer->write(pDOMDocument, theOutputDesc);


}; 





double getValue(ProName proname){
	ProError error; 
	std::ofstream log("logMain.log",std::ofstream::app);
	ProMdl p_model;
	ProModelitem p_model_item; 
	ProParameter param;
	ProParamvalue provalue;
	ProParamvalueType protype;
	double value;



	error = ProMdlCurrentGet(&p_model);
	if (error != 0)
	{
	log << "ProMdlCurrentGet returned" << error << std::endl;
	}
	
		error = ProMdlToModelitem(p_model,&p_model_item);
	if (error != 0)
	{
	log << "ProMdlToModelitem returned" << error << std::endl;
	}
	
	

	error=	ProParameterInit(&p_model_item,proname , &param);
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

	ProParamvalueValueGet	(&provalue, protype, &value);
	if (error != 0)
	{
	log << "ProParamvalueValueGet" << error << std::endl;
	}
	
	//log << "Value" << value << std::endl;


   
   //std::basic_ostringstream<wchar_t> oss;
   //oss << value;
   //std::wstring s = oss.str();
   //const wchar_t* c = s.c_str();

	return value;

	
	log << "function"<<endl;
}

const wchar_t* d2wc(double value){
char buffer [33];

std::string s= std::to_string(value);
const wchar_t* a = xercesc::XMLString::transcode(s.c_str());
return a;

}

const wchar_t* s2wc(string s){
char buffer [33];


const wchar_t* a = xercesc::XMLString::transcode(s.c_str());
return a;

}

	uiCmdCmdActFn Parameter_import(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data)
	{

	std::ofstream log("logMain.log",std::ofstream::app);
	ProName proname;
	
	/////////////////////////////////////////////////	xml auslesen
	
	xercesc::XMLPlatformUtils::Initialize();
	const char* xmlFile = "C:\\Users\\prakti2\\Desktop\\xml2.xml" ;
	XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(gLS);
    DOMLSParser* parser = ((DOMImplementationLS*)impl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	DOMDocument* doc = 0;
	doc = parser->parseURI(xmlFile);
	DOMElement* root = doc->getDocumentElement();//rootElement
	//DOMElement* rootModel;
	string name = XMLString::transcode(root->getNodeName());

	log << "Root Name: " << name << endl;

	struct FRAME {   // Declare FRAME struct type  
    string Name;   // Declare member types  
    double x; 
	double y; 
	double z; 
	double rx; 
	double ry; 
	double rz; 
	} frame;  

	double value;


    std::vector<FRAME> v;
    
    
 // Define object of type   

	for (DOMElement* rootChild = root->getFirstElementChild();rootChild!=0;rootChild=rootChild->getNextElementSibling())
	{
	
		//name = XMLString::transcode(rootChild->getNodeName()); 
			
		
		for (DOMElement* rootModul = rootChild-> getFirstElementChild(); rootModul!=0; rootModul=rootModul->getNextElementSibling())
			{
				 name = XMLString::transcode(rootModul->getNodeName());
				 string text = XMLString::transcode(rootModul->getTextContent());

			

				 if (name.compare("Name") != 0)
				 {
					 const char * c = text.c_str();
					value = atof(c);
				 };

				 


			if (name.compare("Name") == 0)
			{
				frame.Name = text;
			}

			else if (name.compare("x") == 0 )
			{
				frame.x = value;
			}

				else if (name.compare("y") == 0 )
				{
					frame.y = value;
				}

				else if (name.compare("z") == 0 )
				{
					frame.z = value;
				}

				else if (name.compare("rx") == 0 )
				{
					frame.rx = value;
				}

				else if (name.compare("ry") == 0 )
				{
					frame.ry = value;
				}

				else if (name.compare("rz") == 0 )
				{
					frame.rz = value;
					v.push_back(frame);
				}
          


			}
			
		
	}
	
///////////////////////////////////////////////////

	for (int i=0; i<v.size(); i++)
	{
		if (v[i].Name.compare("Kran") == 0)
		{
			ProStringToWstring(proname,"kran_x");
			import(proname, v[i].x);

	
			ProStringToWstring(proname,"kran_y");
			import(proname, v[i].y);

			ProStringToWstring(proname,"kran_z");
			import(proname, v[i].z);
		}
	}
	
	return 0;
	}



//basic function for setting the text content of a element node
void setTextContent(DOMElement* element,string content)
{
	element->setTextContent(xercesc::XMLString::transcode(content.c_str()));
};



	uiCmdCmdActFn Parameter_export(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data)
	{

#include "Values.h"

	ProError error; 
	std::ofstream log("logMain.log",std::ofstream::app);
	ProMdl p_model;
	ProModelitem p_model_item; 
	ProParameter param;
	ProParamvalue provalue;
	ProParamvalueType protype;
	double value;
	vector<Values> values;


		XMLPlatformUtils::Initialize();
	xercesc::XMLPlatformUtils::Initialize();
	const char* xmlFile = "E:\\cad_Li\\files\\xml3.xml" ;
	XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
    DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(gLS);

	struct NAMES {   // Declare FRAME struct type  
    string Name;   // Declare member types  
    wchar_t* x; 
	wchar_t* y; 
	wchar_t* z; 
	wchar_t* rx; 
	wchar_t* ry; 
	wchar_t* rz; 
	} names;



		vector<NAMES> v;
		
		Values a[5];

	names.Name	= "Kran";
	names.x		= L"Kran_x";
	names.y		= L"kran_y";
	names.z		= L"kran_z";
	names.rx	= L"kran_rx";
	names.ry	= L"kran_ry";
	names.rz	= L"kran_rz";

	v.push_back(names);


	names.Name	= "Montageplatte";
	names.x		= L"Montageplatte_x";
	names.y		= L"Montageplatte_y";
	names.z		= L"Montageplatte_z";
	names.rx	= L"Montageplatte_rx";
	names.ry	= L"Montageplatte_ry";
	names.rz	= L"Montageplatte_rz";

	v.push_back(names);

	names.Name	= "OptischerSensor";
	names.x		= L"OptischerSensor_x";
	names.y		= L"OptischerSensor_y";
	names.z		= L"OptischerSensor_z";
	names.rx	= L"OptischerSensor_rx";
	names.ry	= L"OptischerSensor_ry";
	names.rz	= L"OptischerSensor_rz";

	v.push_back(names);


	names.Name	= "Werkstückrückgabe";
	names.x		= L"ABLAGE_X";
	names.y		= L"ABLAGE_Y";
	names.z		= L"ABLAGE_Z";
	names.rx	= L"ABLAGE_RX";
	names.ry	= L"ABLAGE_RY";
	names.rz	= L"ABLAGE_RZ";

	v.push_back(names);

	names.Name	= "xPPU_UseCase1";
	names.x		= L"usecase_x";
	names.y		= L"usecase_y";
	names.z		= L"usecase_z";
	names.rx	= L"usecase_rx";
	names.ry	= L"usecase_ry";
	names.rz	= L"usecase_rz";

	v.push_back(names);


	
	


	error = ProMdlCurrentGet(&p_model);
	if (error != 0)
	{
	log << "ProMdlCurrentGet returned" << error << std::endl;
	}
	
		error = ProMdlToModelitem(p_model,&p_model_item);
	if (error != 0)
	{
	log << "ProMdlToModelitem returned" << error << std::endl;
	}
	

	
	  DOMText* pTextNode = NULL;
  DOMText* pTextNode1 = NULL;
  DOMText* pTextNode2 = NULL;
  DOMText* pTextNode3 = NULL;
  DOMText* pTextNode4 = NULL;
  DOMText* pTextNode5 = NULL;
  DOMText* pTextNode6 = NULL;
DOMElement * pChild = NULL;
		
   DOMDocument *pDOMDocument = NULL;															// leeres DOMDocument wird erstellt 
log << 2<< std::endl;

pDOMDocument = impl->createDocument(0, L"Frame", 0);							// Frame-Element erstellen
log << 3<< std::endl;
DOMElement * pRootElement = NULL;															// Frame als Rootelement festlegen
pRootElement = pDOMDocument->getDocumentElement();
log << 4<< std::endl;
DOMElement * pModulElement = NULL;	

std::wstring s;
	std::basic_ostringstream<wchar_t> oss;
	double x[5],y[5],z[5],rx[5],ry[5],rz[5];

for(int i=0;i<5;i++)
{
	



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

		ProParamvalueValueGet	(&provalue, protype, &x[i]);
		if (error != 0)
		{
		log << "ProParamvalueValueGet" << error << std::endl;
		}
	
				   

	
		log << "Value" << x[i]<< std::endl;
		log<<"d2wc:	"<< d2wc(x[i]) << endl;


	
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

		ProParamvalueValueGet	(&provalue, protype, &y[i]);
		if (error != 0)
		{
		log << "ProParamvalueValueGet" << error << std::endl;
		}
	

	
		log << "Value" << y[i]<< std::endl;
		log<<"d2wc:	"<< d2wc(y[i]) << endl;


	
	
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

		ProParamvalueValueGet	(&provalue, protype, &z[i]);
		if (error != 0)
		{
		log << "ProParamvalueValueGet" << error << std::endl;
		}
	
		 		 
	
		log << "Value" << z[i]<< std::endl;
		log<<"d2wc:	"<< d2wc(z[i]) << endl;


	
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

		ProParamvalueValueGet	(&provalue, protype, &rx[i]);
		if (error != 0)
		{
		log << "ProParamvalueValueGet" << error << std::endl;
		}
	
	
		log << "Value" << rx[i]<< std::endl;
		log<<"d2wc:	"<< d2wc(rx[i]) << endl;


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

		ProParamvalueValueGet	(&provalue, protype, &ry[i]);
		if (error != 0)
		{
		log << "ProParamvalueValueGet" << error << std::endl;
		}
	
				   
	
		log << "Value" <<ry[i]<< std::endl;
		log<<"d2wc:	"<< d2wc(ry[i]) << endl;



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

		ProParamvalueValueGet	(&provalue, protype, &rz[i]);
		if (error != 0)
		{
		log << "ProParamvalueValueGet" << error << std::endl;
		}
	
	
		log << "Value" <<rz[i]<< std::endl;
		log<<"d2wc:	"<< d2wc(rz[i]) << endl;

		



//  Modulknoten erstellen												

	pModulElement = pDOMDocument->createElement(L"Modul");
 pRootElement->appendChild(pModulElement);
 log << 5<< std::endl;


 pModulElement->setAttribute(L"test",L"1");
 pChild = pDOMDocument->createElement(L"Name");
    pTextNode=pDOMDocument->createTextNode(s2wc(v[i].Name));
    pChild->appendChild(pTextNode);
pModulElement->appendChild(pChild);
log << 6<< std::endl;

		pChild = pDOMDocument->createElement(L"x");
    pTextNode=pDOMDocument->createTextNode(d2wc(x[i]));
    pChild->appendChild(pTextNode);
pModulElement->appendChild(pChild);
log << 7<< std::endl;


pChild = pDOMDocument->createElement(L"y");
    pTextNode2=pDOMDocument->createTextNode(d2wc(y[i]));
    pChild->appendChild(pTextNode2);
pModulElement->appendChild(pChild);
log << 6<< std::endl;

pChild = pDOMDocument->createElement(L"z");
    pTextNode3=pDOMDocument->createTextNode(d2wc(z[i]));
    pChild->appendChild(pTextNode3);
pModulElement->appendChild(pChild);
log << 7<< std::endl;

pChild = pDOMDocument->createElement(L"rx");
    pTextNode3=pDOMDocument->createTextNode(d2wc(rx[i]));
    pChild->appendChild(pTextNode3);
pModulElement->appendChild(pChild);
log << 8<< std::endl;


pChild = pDOMDocument->	createElement(L"ry");
    pTextNode3=pDOMDocument->createTextNode(d2wc(ry[i]));
    pChild->appendChild(pTextNode3);
pModulElement->appendChild(pChild);
log << 9<< std::endl;


pChild = pDOMDocument->createElement(L"rz");
    pTextNode3=pDOMDocument->createTextNode(d2wc(rz[i]));
    pChild->appendChild(pTextNode3);
	log << 11<< std::endl;
pModulElement->appendChild(pChild);
log << 10<< std::endl;
}
	

	

	

DOMLSSerializer *writer = ((DOMImplementationLS*)impl)->createLSSerializer();
DOMLSOutput *theOutputDesc = ((DOMImplementationLS*)impl)->createLSOutput();
XMLFormatTarget *formTarget = new LocalFileFormatTarget((XMLCh*)L"E:\\cad_Li\\files\\export.xml");

theOutputDesc->setByteStream(formTarget);
writer->getDomConfig()->setParameter(L"format-pretty-print",true);
writer->write(pDOMDocument, theOutputDesc);


	////ProStringToWstring(proname,"kran_x");
	////
	////value=getValue(proname);
	////log << "Value" << value << std::endl;
		//DOMDocument*   Doc;



	




   
	/*const wchar_t* a= ws.c_str();*/


	// Pointer to our DOMImplementation.
	//DOMImplementation * pDOMImplementation = NULL;


	// Pointer to a DOMDocument.




//

//pChild = pDOMDocument->createElement(L"rx");
//    pTextNode4=pDOMDocument->createTextNode((XMLCh*)a);
//    pChild->appendChild(pTextNode4);
//pModulElement->appendChild(pChild);
//
//pChild = pDOMDocument->createElement(L"ry");
//    pTextNode5=pDOMDocument->createTextNode((XMLCh*)a);
//    pChild->appendChild(pTextNode5);
//pModulElement->appendChild(pChild);
//
//pChild = pDOMDocument->createElement(L"rz");
//    pTextNode6=pDOMDocument->createTextNode((XMLCh*)a);
//    pChild->appendChild(pTextNode6);
//pModulElement->appendChild(pChild);



	


// Writer
//DOMLSSerializer *writer = ((DOMImplementationLS*)impl)->createLSSerializer();
//DOMLSOutput *theOutputDesc = ((DOMImplementationLS*)impl)->createLSOutput();
//XMLFormatTarget *formTarget = new LocalFileFormatTarget((XMLCh*)L"C:\\Users\\prakti2\\Desktop\\export.xml");
//
//theOutputDesc->setByteStream(formTarget);
//writer->getDomConfig()->setParameter(L"format-pretty-print",true);
//writer->write(pDOMDocument, theOutputDesc);



	return 0;
	}

