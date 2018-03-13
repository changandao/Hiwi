//#include "Values.h"
//
//
//	struct Values::NAMES {   // Declare FRAME struct type  
//    XMLCh* Name;   // Declare member types  
//     wchar_t* x; 
//	XMLCh* y; 
//	XMLCh* z; 
//	XMLCh* rx; 
//	XMLCh* ry; 
//	XMLCh* rz; 
//	} names;
//	 wchar_t* x = L"kran_x";
//
//	vector<Values::NAMES> Values::buildingVector(/*struct Values::NAMES names*/){
//
//	struct Values::NAMES {   // Declare FRAME struct type  
//    XMLCh* Name;   // Declare member types  
//     wchar_t* x; 
//	XMLCh* y; 
//	XMLCh* z; 
//	XMLCh* rx; 
//	XMLCh* ry; 
//	XMLCh* rz; 
//	} names;
//	 wchar_t* x = L"kran_x";
//		vector<Values::NAMES> v;
//
//	names.Name	= L"Kran";
//	names.x		= x;
//	names.y		= L"kran_y";
//	names.z		= L"kran_z";
//	names.rx	= L"kran_rx";
//	names.ry	= L"kran_ry";
//	names.rz	= L"kran_rz";
//
//	v.push_back(names);
//
//
//	names.Name	= L"OptischerSensor";
//	names.x		= L"OptischerSensor_x";
//	names.y		= L"OptischerSensor_y";
//	names.z		= L"OptischerSensor_z";
//	names.rx	= L"OptischerSensor_rx";
//	names.ry	= L"OptischerSensor_ry";
//	names.rz	= L"OptischerSensor_rz";
//
//	v.push_back(names);
//
//
//	return v;
//
//		}; 
//
//
//	double Value(vector<Values::NAMES> v){
//	ProError error; 
//	std::ofstream log("logMain.log",std::ofstream::app);
//	ProMdl p_model;
//	ProModelitem p_model_item; 
//	ProParameter param;
//	ProParamvalue provalue;
//	ProParamvalueType protype;
//	double value;
//	
//
//
//	error = ProMdlCurrentGet(&p_model);
//	if (error != 0)
//	{
//	log << "ProMdlCurrentGet returned" << error << std::endl;
//	}
//	
//		error = ProMdlToModelitem(p_model,&p_model_item);
//	if (error != 0)
//	{
//	log << "ProMdlToModelitem returned" << error << std::endl;
//	}
//	
//	
//
//	error=	ProParameterInit(&p_model_item,v[0].x , &param);
//	if (error != 0)
//	{
//	log << "ProParameterInit returned" << error << std::endl;
//	}
//
//
//	
//	ProParameterValueGet	(&param, &provalue);
//	if (error != 0)
//	{
//	log << "ProParameterValueGet" << error << std::endl;
//	}
//
//
//	ProParamvalueTypeGet	(&provalue, &protype);
//	if (error != 0)
//	{
//	log << "ProParamvalueTypeGet" << error << std::endl;
//	}
//
//	ProParamvalueValueGet	(&provalue, protype, &value);
//	if (error != 0)
//	{
//	log << "ProParamvalueValueGet" << error << std::endl;
//	}
//	
//	//log << "Value" << value << std::endl;
//
//
//   
//   //std::basic_ostringstream<wchar_t> oss;
//   //oss << value;
//   //std::wstring s = oss.str();
//   //const wchar_t* c = s.c_str();
//
//	return value;
//
//	
//	log << "function"<<endl;
//}