#include "main.h"

static uiCmdAccessState TestAccessDefault(uiCmdAccessMode access_mode)
{
	return (ACCESS_AVAILABLE);
}


extern "C" int user_initialize( int argc, char *argv[], char *proe_vsn, char *build, wchar_t err_buff[] )
{
	std::ofstream log("logMain.log",std::ofstream::app);  //// Log-Datei zur Fehlersuche

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

	////////////////////// Button für Import in Creo


	// Festlegen,welche Funktion durch den Button aufgerufen wird
	error = ProCmdActionAdd(
		"Model_import", 
		(uiCmdCmdActFn) Model_import, 
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
		"Model_import1",
		"Model_import2",	
		"Model_import3",
		L"menu.txt"	
		);
	if(error != 0)
	{
		log << "ProCmdDesignate for for_Model_import returned " << error << std::endl;
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
	std::ofstream log("logMain.log",std::ofstream::app);
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

	double value;

	DOMText* pTextNode = NULL;
	DOMElement * pChild = NULL;	
	DOMElement * pAttr1 = NULL;
	DOMElement * pAttr2 = NULL;
	DOMElement * pValue = NULL;
	DOMDocument *pDOMDocument = NULL;
	string axises[] = {"x","y","z","rz","ry","rz",}; 
	string parts[] = {"PPU","Kran", "Sortierstrecke", "Stapel", "Stempel", "Montageplatte"};
	int num_elem = 0;
	/// Vektor vom Typ Frame in dem alle Werte gespeichert werden
	std::vector<FRAME> v;
	DOMElement * pInstanceElement = root->getFirstElementChild(); 
	string instanceName = XMLString::transcode(pInstanceElement->getAttribute(L"Name")); //InstanceHierarchy lesen
	parts[0] = instanceName;
	log<<"InstanceName is: "<< parts[0]<<endl;
	for(DOMElement* pChild = pInstanceElement->getFirstElementChild(); pChild!=0; pChild = pChild->getNextElementSibling())
	{
		string attr_part = XMLString::transcode(pChild->getAttribute(L"Name"));//InternalElement Name lesen
		for(DOMElement* pChild_part = pChild->getFirstElementChild(); pChild_part!=0; pChild_part = pChild_part->getNextElementSibling())
		{
			string name_child_part = XMLString::transcode(pChild_part->getTagName());
			if(name_child_part.compare("Attribute")==0)
			{
				child_frame.Name = attr_part;
				num_elem++;
				parts[num_elem] = attr_part;
				for(DOMElement * pAttr = pChild_part->getFirstElementChild(); pAttr!=0; pAttr=pAttr->getNextElementSibling())
				{
					attr_part = XMLString::transcode(pAttr->getAttribute(L"Name"));
					DOMElement * pValue = pAttr->getFirstElementChild();
					string text = XMLString::transcode(pValue->getTextContent());// Value von der Axis lesen
					const char * c = text.c_str();
					value = atof(c);
					// die Value in die struct store
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
			ProStringToWstring(proname,"Sortierstrecke_frame_x");
			import(proname, v[i].x);

			ProStringToWstring(proname,"Sortierstrecke_frame_y");
			import(proname, v[i].y);

			ProStringToWstring(proname,"Sortierstrecke_frame_z");
			import(proname, v[i].z);

			ProStringToWstring(proname,"Sortierstrecke_frame_rx");
			import(proname, v[i].rx);

			ProStringToWstring(proname,"Sortierstrecke_frame_ry");
			import(proname, v[i].ry);

			ProStringToWstring(proname,"Sortierstrecke_frame_rz");
			import(proname, v[i].rz);
		}else if (v[i].Name.compare(parts[3]) == 0)
		{
			ProStringToWstring(proname,"Stapel_frame_x");
			import(proname, v[i].x);

			ProStringToWstring(proname,"Stapel_frame_y");
			import(proname, v[i].y);

			ProStringToWstring(proname,"Stapel_frame_z");
			import(proname, v[i].z);

			ProStringToWstring(proname,"Stapel_frame_rx");
			import(proname, v[i].rx);

			ProStringToWstring(proname,"Stapel_frame_ry");
			import(proname, v[i].ry);

			ProStringToWstring(proname,"Stapel_frame_rz");
			import(proname, v[i].rz);
		}else if (v[i].Name.compare(parts[4]) == 0)
		{
			ProStringToWstring(proname,"Stempel_frame_x");
			import(proname, v[i].x);

			ProStringToWstring(proname,"Stempel_frame_y");
			import(proname, v[i].y);

			ProStringToWstring(proname,"Stempel_frame_z");
			import(proname, v[i].z);

			ProStringToWstring(proname,"Stempel_frame_rx");
			import(proname, v[i].rx);

			ProStringToWstring(proname,"Stempel_frame_ry");
			import(proname, v[i].ry);

			ProStringToWstring(proname,"Stempel_frame_rz");
			import(proname, v[i].rz);
		}else if (v[i].Name.compare(parts[5]) == 0)
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
		}
	}

	return 0;

}

uiCmdCmdActFn Model_import(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data)
{

	std::ofstream log("logMain.txt",std::ofstream::app);
	ProName proname;

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

	double value;

	DOMText* pTextNode = NULL;
	DOMElement * pChild = NULL;	
	DOMElement * pAttr1 = NULL;
	DOMElement * pAttr2 = NULL;
	DOMElement * pValue = NULL;
	DOMDocument *pDOMDocument = NULL;
	string axises[] = {"x","y","z","rz","ry","rz",}; 
	string parts[] = {"PPU","Kran", "Sortierstrecke", "Stapel", "Stempel", "Montageplatte"};

	/// Vektor vom Typ Frame in dem alle Werte gespeichert werden
	std::vector<FRAME> v;
	DOMElement * pInstanceElement = root->getFirstElementChild(); 
	string instanceName = XMLString::transcode(pInstanceElement->getAttribute(L"Name")); //InstanceHierarchy lesen
	parts[0] = instanceName;
	log<<"InstanceName is: "<< parts[0]<<endl;
	for(DOMElement* pChild = pInstanceElement->getFirstElementChild(); pChild!=0; pChild = pChild->getNextElementSibling())
	{
		string attr_part = XMLString::transcode(pChild->getAttribute(L"Name"));//Attribute lesen
		for(DOMElement* pChild_part = pChild->getFirstElementChild(); pChild_part!=0; pChild_part = pChild_part->getNextElementSibling())
		{
			string name_child_part = XMLString::transcode(pChild_part->getTagName());
			if(name_child_part.compare("Attribute")==0)
			{
				child_frame.Name = attr_part;
				for(DOMElement * pAttr = pChild_part->getFirstElementChild(); pAttr!=0; pAttr=pAttr->getNextElementSibling())
				{
					attr_part = XMLString::transcode(pAttr->getAttribute(L"Name"));
					DOMElement * pValue = pAttr->getFirstElementChild();
					string text = XMLString::transcode(pValue->getTextContent());// Value von der Axis lesen
					const char * c = text.c_str();
					value = atof(c);
					// die Value in die struct store
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
			ProStringToWstring(proname,"Sortierstrecke_frame_x");
			import(proname, v[i].x);

			ProStringToWstring(proname,"Sortierstrecke_frame_y");
			import(proname, v[i].y);

			ProStringToWstring(proname,"Sortierstrecke_frame_z");
			import(proname, v[i].z);

			ProStringToWstring(proname,"Sortierstrecke_frame_rx");
			import(proname, v[i].rx);

			ProStringToWstring(proname,"Sortierstrecke_frame_ry");
			import(proname, v[i].ry);

			ProStringToWstring(proname,"Sortierstrecke_frame_rz");
			import(proname, v[i].rz);
		}else if (v[i].Name.compare(parts[3]) == 0)
		{
			ProStringToWstring(proname,"Stapel_frame_x");
			import(proname, v[i].x);

			ProStringToWstring(proname,"Stapel_frame_y");
			import(proname, v[i].y);

			ProStringToWstring(proname,"Stapel_frame_z");
			import(proname, v[i].z);

			ProStringToWstring(proname,"Stapel_frame_rx");
			import(proname, v[i].rx);

			ProStringToWstring(proname,"Stapel_frame_ry");
			import(proname, v[i].ry);

			ProStringToWstring(proname,"Stapel_frame_rz");
			import(proname, v[i].rz);
		}else if (v[i].Name.compare(parts[4]) == 0)
		{
			ProStringToWstring(proname,"Stempel_frame_x");
			import(proname, v[i].x);

			ProStringToWstring(proname,"Stempel_frame_y");
			import(proname, v[i].y);

			ProStringToWstring(proname,"Stempel_frame_z");
			import(proname, v[i].z);

			ProStringToWstring(proname,"Stempel_frame_rx");
			import(proname, v[i].rx);

			ProStringToWstring(proname,"Stempel_frame_ry");
			import(proname, v[i].ry);

			ProStringToWstring(proname,"Stempel_frame_rz");
			import(proname, v[i].rz);
		}else if (v[i].Name.compare(parts[5]) == 0)
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
		}
	}

	return 0;

}

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

		error = ProMdlnameRetrieve((depd+i)->name, *(files+i), &p_model);//retrieve current modle from name and type

		if(typeHash[*(files+i)] == prt_name)
			continue;
		else
			addTrees(&child, p_model, typeHash);//recursivel generatetree
	}
}

int SequenceSearch(const vector<string> &part_names, string Paraname)
{
	int i;
	int size = part_names.size();
	for(i=0; i<size; i++)
		if(part_names[i]==Paraname)
			return i-1;
	return -1;
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
	// leeres DOMDocument wird erstellt
	//XMLCh axises[6] = {L"x", L"y"}//, L"y", L"z", L"rx", L"ry", L"rz"};

	///DOMDocument erstellen und Rootelement festlegen
	pDOMDocument = impl->createDocument(0, L"CAEXFile", 0);	// Root erstellen
	DOMElement * pRootElement = NULL;															
	pRootElement = pDOMDocument->getDocumentElement();// File name als Child von Rootelement festlegen
	pRootElement->setAttribute(L"FileName", L"PPU.aml");   
	pRootElement->setAttribute(L"SchemaVersion", L"2.15");
	DOMElement * pInstanceElement = NULL;	

	XMLCh *InstanceName = XMLString::transcode(parts[0].c_str());
	pInstanceElement = pDOMDocument->createElement(L"InstanceHierarchy"); //InstanceHierarchy erstellen
	pInstanceElement->setAttribute(L"Name", InstanceName);
	XMLString::release(&InstanceName);
	pRootElement->appendChild(pInstanceElement); // die InstanceHierarchy und File verbinden

	saveInternalElement(filetree, pDOMDocument, &pInstanceElement, log, parts,axisNumber);
	//generate DOM trees of all models

	/// xml datei schreiben
	DOMLSSerializer *writer = ((DOMImplementationLS*)impl)->createLSSerializer();
	DOMLSOutput *theOutputDesc = ((DOMImplementationLS*)impl)->createLSOutput();
	XMLFormatTarget *formTarget = new LocalFileFormatTarget((XMLCh*)L"..\\..\\files\\newexport.aml");
	cout<<"E:\\cad_Li\\files\\newexport.xml"<<endl;

	theOutputDesc->setByteStream(formTarget);
	writer->getDomConfig()->setParameter(L"format-pretty-print", true);
	writer->write(pDOMDocument, theOutputDesc);
}

void saveInternalElement(TreeNode* filetree, DOMDocument *pDOMDocument, DOMElement **pInstanceElement, ofstream &log,
						 vector<string> parts, double **axisNumber)
{
	DOMElement * pChild = NULL;
	DOMElement * pAttr1 = NULL;
	DOMElement * pAttr2 = NULL;
	DOMElement * pValue = NULL;
	DOMText* pTextNode = NULL;
	string axises[] = {"x","y","z","rx","ry","rz",};

	string child_name;
	int paraind = 0;
	//vector<TreeNode *> children = filetree->getChild;
	for(int i = 0; i < filetree->childrenNumber(); i++)
	{
		TreeNode *child = filetree->getChild(i);
		child_name = child->getTextContent();
		pChild = pDOMDocument->createElement(L"InternalElement");
		log<<"creating name attribute: "<< child_name << endl;
		pChild->setAttribute(L"Name", s2wc(child_name));
		log<<"creating type attribute: "<< child->getType() << endl;
		pChild->setAttribute(L"Type", s2wc(child->getType()));

		//set the frame and its axies
		paraind = SequenceSearch(parts, child_name);
		if(paraind != -1)
		{
			pAttr1 = pDOMDocument->createElement(L"Attribute");
			pAttr1->setAttribute(L"Name", L"Frame");
			for(int j = 0; j < 6; j++)
			{
				//Attribute
				pAttr2 = pDOMDocument->createElement(L"Attribute");
				pAttr2->setAttribute(L"AttributeDataType", L"xs:double");
				pAttr2->setAttribute(L"Name", s2wc(axises[j]));

				pValue = pDOMDocument->createElement(L"Value");
				pTextNode=pDOMDocument->createTextNode(d2wc(axisNumber[j][paraind])); //Text Node erstellen
				pValue->appendChild(pTextNode); //Node verbinden
				pAttr2->appendChild(pValue);
				pAttr1->appendChild(pAttr2);
				pChild->appendChild(pAttr1);
			}

			// ====*** if you want only generate modle trees only for parameters matched models to DOM trees, uncomment lines below

			(*pInstanceElement)->appendChild(pChild);
			if(child->hasChildren())
				saveInternalElement(child, pDOMDocument, &pChild, log, parts, axisNumber);
			else
				continue;

			// ====***
		}

		// ====*** if you want generate whole modle trees to DOM trees, uncomment lines below

		/*(*pInstanceElement)->appendChild(pChild);
		if(child->hasChildren())
		saveInternalElement(child, pDOMDocument, &pChild, log, parts, axisNumber);
		else
		continue;*/

		// ====***

	}
	//delete pChild;

}


/// Funktion für den Export der Dateien
uiCmdCmdActFn Parameter_export(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data)
{

	ProError error; 
	std::ofstream log("logMain.log",std::ofstream::app);
	ProMdl p_model;
	ProModelitem p_model_item; 
	ProParameter param;
	ProParamvalue provalue;
	//	ProParamvalueType protype;
	ProMdlType promdltype = ProMdlType::PRO_MDL_ASSEMBLY; //= PRO_ASSEMBLY;
	ProMdl  *p_model_array = NULL;

	// define a hash to represent promdlfiletype as normal string
	map<ProMdlfileType, string> typeHash;
	typeHash[ProMdlfileType::PRO_MDLFILE_ASSEMBLY] = asm_name;
	typeHash[ProMdlfileType::PRO_MDLFILE_PART] = prt_name;

	vector<Values> values;
	vector<string> parts;
	//	string parts[] = {"PPU","Kran", "Sortierstrecke", "Stapel", "Stempel", "Montageplatte"};

	///Export the model
	error = ProMdlCurrentGet(&p_model);
	if (error != 0)
	{
		log << "ProMdlCurrentGet returned" << error << std::endl;
	}

	ProMdlName test_name;
	error = ProMdlMdlnameGet(p_model, test_name);
	parts.push_back(match_pattern(test_name));
	TreeNode *mdltree =new TreeNode(w2string(test_name), typeHash[ProMdlfileType::PRO_MDLFILE_ASSEMBLY]);
	//error = ProSessionMdlList(promdltype, &p_model_array, &p_model_num);

	ProMdlnameShortdata *depd = NULL;
	ProMdlfileType * files = NULL;
	int num_pdep = 0;
	error = ProMdlDependenciesDataList(p_model, &depd, &files, &num_pdep);

	log<<"the third test"<< endl;

	string bauObject;
	string delimiter = "[[:alpha:]]*";
	for(int i = 0; i<num_pdep; i++)
	{
		wcscpy(test_name, (*(depd+i)).name);
		bauObject = match_pattern(test_name);
		parts.push_back(bauObject);
		log<< "the name is  " << bauObject << std::endl;
	}
	
	parts = delRep(parts);//delete all repetetive parts

	vector<NAMES> v;
	int c = 5;

	names.Name	= parts[2];
	names.x		= L"Kran_frame_x";
	names.y		= L"kran_frame_y";
	names.z		= L"kran_frame_z";
	names.rx	= L"kran_frame_rx";
	names.ry	= L"kran_frame_ry";
	names.rz	= L"kran_frame_rz";

	v.push_back(names);

	names.Name	= parts[3];
	names.x		= L"Sortierstrecke_frame_x";
	names.y		= L"Sortierstrecke_frame_y";
	names.z		= L"Sortierstrecke_frame_z";
	names.rx	= L"Sortierstrecke_frame_rx";
	names.ry	= L"Sortierstrecke_frame_ry";
	names.rz	= L"Sortierstrecke_frame_rz";


	v.push_back(names);

	names.Name	= parts[4];
	names.x		= L"Stapel_frame_x";
	names.y		= L"Stapel_frame_y";
	names.z		= L"Stapel_frame_z";
	names.rx	= L"Stapel_frame_rx";
	names.ry	= L"Stapel_frame_ry";
	names.rz	= L"Stapel_frame_rz";

	v.push_back(names);


	names.Name	=parts[5];
	names.x		= L"Stempel_frame_x";
	names.y		= L"Stempel_frame_y";
	names.z		= L"Stempel_frame_z";
	names.rx	= L"Stempel_frame_rx";
	names.ry	= L"Stempel_frame_ry";
	names.rz	= L"Stempel_frame_rz";

	v.push_back(names);

	names.Name	= parts[1];
	names.x		= L"Montageplatte_frame_x";
	names.y		= L"Montageplatte_frame_y";
	names.z		= L"Montageplatte_frame_z";
	names.rx	= L"Montageplatte_frame_rx";
	names.ry	= L"Montageplatte_frame_ry";
	names.rz	= L"Montageplatte_frame_rz";

	v.push_back(names);

	error = ProMdlToModelitem(p_model,&p_model_item);
	if (error != 0)
	{
		log << "ProMdlToModelitem returned" << error << std::endl;
	}

	//define a 2d arrays to store all parameters 
	int num_axis = 6;
	double **axisNumber;  
	axisNumber = new double *[num_axis];  
	for(int i=0;i<num_axis;i++)  
	{  
		axisNumber[i]=new double [c];  
		memset(axisNumber[i],0,(c)*sizeof(double)); // 
	}  
	//double axisNumber[6][4];
	//double x[5],y[5],z[5],rx[5],ry[5],rz[5];

	/// Parameterwerte werden eingelesen, in arrays gespeichert und in xml-Datei füllen
	for(int i=0;i<c;i++)
	{
		/// x
		error=	ProParameterInit(&p_model_item,v[i].x , &param);
		if (error != 0)
		{
			log << "ProParameterInit returned" << error << std::endl;
		}


		error = ProParameterValueGet	(&param, &provalue);
		if (error != 0)
		{
			log << "ProParameterValueGet" << error << std::endl;
		}

		axisNumber[0][i] = provalue.value.d_val;

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

		axisNumber[1][i] = provalue.value.d_val;


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

		axisNumber[2][i] = provalue.value.d_val;

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

		axisNumber[3][i] = provalue.value.d_val;

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

		axisNumber[4][i] = provalue.value.d_val;


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
		axisNumber[5][i] = provalue.value.d_val;
	}

	//generate model tree
	addTrees(&mdltree, p_model, typeHash);
	
	// generate and write AML file to local disk
	writeAml(mdltree, v, parts, axisNumber, c);

	return 0;

}

