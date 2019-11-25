#ifndef _voxelquest_json_h_
#define _voxelquest_json_h_


enum JSONType { JSONType_Null, JSONType_String, JSONType_Bool, JSONType_Number, JSONType_Array, JSONType_Object };

class JSONValue;

typedef std::vector<JSONValue*> JSONArray;
typedef std::map<std::string, JSONValue*> JSONObject;
typedef std::map<std::string, JSONValue*>::iterator joi_type;

class JSONValue
{
	
	public:
		JSONValue(/*NULL*/);
		JSONValue(const char *m_char_value);
		JSONValue(const std::string &m_string_value);
		JSONValue(bool m_bool_value);
		JSONValue(double m_number_value);
		JSONValue(const JSONArray &m_array_value);
		JSONValue(const JSONObject &m_object_value);
		~JSONValue();

		bool IsNull() const;
		bool IsString() const;
		bool IsBool() const;
		bool IsNumber() const;
		bool IsArray() const;
		bool IsObject() const;
		
		const std::string &AsString() const;
		bool AsBool() const;
		double AsNumber() const;
		const JSONArray &AsArray() const;
		const JSONObject &AsObject() const;

		int CountChildren() const;
		
		bool HasChild(int index) const;
		JSONValue *Child(int index);
		
		bool HasChild(const char* name) const;
		JSONValue *Child(const char* name);
		
		bool HasChild(string name) const;
		JSONValue *Child(string name);
		
		void RemoveChild(string name);

		std::string Stringify(int curLev) const;

		static JSONValue *Parse(const char **data);

		static std::string StringifyString(const std::string &str);
	
		//JSONValue* insertValue(string name, JSONValue* val);
	
		JSONType type;
		std::string string_value;
		bool bool_value;
		double number_value;
		JSONArray array_value;
		JSONObject object_value;
		string lastKey;
};



class JSON
{
	
	public:
		static JSONValue* Parse(const char *data);
		static std::string Stringify(const JSONValue *value);

		static bool SkipWhitespace(const char **data);
		static bool ExtractString(const char **data, std::string &str);
		static double ParseInt(const char **data);
		static double ParseDecimal(const char **data);

		JSON();
};




// Macros to free an array/object
#define FREE_ARRAY(x) { JSONArray::iterator iter; for (iter = x.begin(); iter != x.end(); iter++) { delete *iter; } }
#define FREE_OBJECT(x) { JSONObject::iterator iter; for (iter = x.begin(); iter != x.end(); iter++) { delete (*iter).second; } }



#endif//_voxelquest__h_