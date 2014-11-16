"""
Generate messages types from .xml metadata
"""

import xml.etree.ElementTree as et
class xml_message :
	def __init__(self, file_path) :
		self.path = file_path
		self.name = None
		self.attribs = list()
		self.hash = None
	def parse(self) :
		parsed = False
		if(utils.file.is_file(self.path)) :
			self.tree = et.parse(self.path)
			if(not self.__set_name()) :
				utils.print.error("Could not set xml name")
			elif(not self.__set_attributes()) :
				utils.print.error("Could not set xml attributes")
			elif(not self.__set_hash()) :
				utils.print.error("Could not set xml hash")
			else :
				utils.print.info("Parsed: " + os.path.basename(self.path))

	def __set_hash(self) :
		if(None != self.name and len(self.attribs) > 0) :
			hash_buff = utils.digest.sha256(self.name)
			for attrib in self.attribs : 
				hash_buff += attrib.hash
			self.hash = utils.digest.sha256(hash_buff)
		setted = None != self.hash
		return setted
	def get_hash(self) :
		return self.hash

	def __set_name(self) :
		elem = self.__get_elements("name")
		setted = None != elem
		if(setted and len(elem) > 0) :
			self.name = self.__get_element_text(elem[0])
		return setted
	def get_name(self) :
		return self.name

	def __set_attributes(self) :
		elem_attribs = self.__get_elements("attributes")
		setted = None != elem_attribs
		if(setted and 1 == len(elem_attribs)) :
			for elem in elem_attribs[0] :
				name = self.__get_element_text(self.__get_attribute("name", elem, 1))
				type = self.__get_element_text(self.__get_attribute("type", elem, 1))
				comments = self.__get_element_text(self.__get_attribute("comments", elem, 1))
				self.attribs.append(xml_attribute(name, type, comments))
		return setted
	def get_attributes(self) :
		return self.attribs

	def __get_attribute(self, attrib, root, count) :
		value = None
		elem_attrib = self.__get_elements(attrib, root)
		if(None != elem_attrib and count == len(elem_attrib)) :
			value = elem_attrib[0]
		return value

	def __get_elements(self, element_name, root = None) :
		if(None == root) :
			root = self.tree
		return root.findall(element_name)
	def __get_element_text(self, element) :
		value = ""
		if(None != element.text) :
			value = element.text.strip()
		return value		

class xml_attribute :
	def __init__(self, id, type, comments) :
		self.id = id
		self.type = type
		self.comments = comments
		self.hash = utils.digest.sha256(utils.digest.sha256(id) + utils.digest.sha256(type) + utils.digest.sha256(comments))

import os, hashlib, time
class utils :
	class print :
		def info(msg) :
			print("[*] " + msg )
		def error(msg) :
			print("[!] " + msg)
	class file :
		def __print_error(msg, path) :
			utils.print.error(msg + ": " + path)
		def is_file(path, silent = False) :
			exists = os.path.isfile(path)
			if(not silent and not exists) :
				utils.file.__print_error("Path doesn't refer to a file", path)
			return exists
		def exists(path, silent = False) :
			exists = os.path.exists(path)
			if(not silent and not exists) :
				utils.file.__print_error("Path doesn't exist", path)
			return exists
		def write(path, content) :
			#utils.print.info("Writting " + path)
			file = open(path, "w")
			file.write(content)
			file.close()
		def write_header(outdir, xml) :
			define = xml.get_name().upper() + "_H"

			content = utils.file_content()
			content.add_line(0, "#ifndef " + define)
			content.add_line(0, "#define " + define)
			content.add_line(0, utils.const.generated_warning)
			content.add_line(0, "#include \"../../cacoon/message.h\"")
			content.add_line(0, "namespace cacoon {")
			content.add_line(1, "namespace comms {")
			content.add_line(2, "struct " + xml.get_name() + " : public serializable {")
			content.add_line(3, "static const " + utils.const.namespace_type + utils.const.hash.type + " " + utils.const.hash.name + "[65];")
			
			attribs = xml.get_attributes();
			for attrib in attribs :
				comments = ""
				if "" != attrib.comments :
					content.add_line(3, "// " + attrib.comments)
				content.add_line(3, utils.const.namespace_type + attrib.type + " " + attrib.id + ";")

			params = "decltype({0}){2} v{1}"
			init = "{0}(v{1})"
			format_if = "('|' == is.get() && is >> v{0})"
			format_shared_ptr = "v{0}"
			constructor_params = ""
			constructor_init = ""
			deserialize_vars = ""
			deserialize_if = ""
			shared_ptr = ""
			for i in range(len(attribs)) :
				if i > 0 :
					constructor_params += ", "
					constructor_init += ", "
					deserialize_vars += "; "
					deserialize_if += " && "
					shared_ptr += ", "
				constructor_params += "const " + params.format(attribs[i].id, i, "&")
				deserialize_vars += params.format(attribs[i].id, i, "")
				constructor_init += init.format(attribs[i].id, i)
				deserialize_if += format_if.format(i)
				shared_ptr += format_shared_ptr.format(i)

			content.add_line(3, xml.get_name() + "({}) ".format(constructor_params))
			content.add_line(3, " : {}".format(constructor_init) + " { }")
			content.add_line(3, "static std::shared_ptr<serializable> deserialize(std::istream& is) {")
			content.add_line(4, "std::shared_ptr<" + xml.get_name() + "> ptr;")
			content.add_line(4, deserialize_vars + ";")
			content.add_line(4, "if(" + deserialize_if + ") {")
			content.add_line(5, "ptr = std::make_shared<" + xml.get_name() + ">(" + shared_ptr + ");")
			content.add_line(4, "}")
			content.add_line(4, "return ptr;")
			content.add_line(3, "}")
			content.add_line(2, "private:")
			content.add_line(3, "virtual void serialize_type(std::ostream& os) const override {")
			attrs = "os << " + utils.const.hash.name
			for attrib in xml.get_attributes() :
				attrs += " << '|' << " + attrib.id
			content.add_line(4, attrs + ";")
			content.add_line(3, "}")
			content.add_line(2, "};")
			content.add_line(1, "}")
			content.add_line(0, "}")
			content.add_line(0, "#endif " + define)
			utils.file.write(outdir + xml.get_name() + ".h", content.get_buffer())

	class file_content :
		def __init__(self) :
			self.buffer = ""
		def add_tab(self, count) :
			for i in range(count) :
				self.buffer += "\t"
		def add_line(self, tabs = 0, content = "") :
			self.add_tab(tabs)
			self.buffer += content + "\n"
		def get_buffer(self) :
			return self.buffer
	class digest :
		def sha256(string) :
			bstr = bytes(string, "ascii")
			h = hashlib.sha256(bstr)
			d = h.hexdigest()
			return d
	class const :
		generated_warning = "/* AUTO-GENERATED FILE, MODIFY AT YOUR OWN RISK */\n/* GENERATED ON: " + time.strftime("%a, %d %b %Y %H:%M:%S", time.localtime()) + "*/"
		namespace_type = "types::"
		class hash :
			type = "charU8"
			name = "id"

class common_header :
	name = "messages_types_includes"
	def __init__(self, outdir) :
		self.path = outdir + common_header.name + ".h"
		self.__init_content()
	def __del__(self) :
		self.__del_content()
		utils.file.write(self.path, self.content)

	def __init_content(self) :
		self.content = "#ifndef " + common_header.name.upper() + "_H\n"
		self.content += "#define " + common_header.name.upper() + "_H\n"
		self.content += utils.const.generated_warning + "\n"
	def __del_content(self) :
		self.content += "#endif\n"

	def add(self, xml) :
		self.content += "#include \"" + xml.get_name() + ".h\"\n"

class common_impl :
	def __init__(self, dir) :
		self.name = "messages_types_implementation"
		self.path = dir + self.name + ".cpp"
		self.__init_content()
	def __del__(self) :
		self.__del_content()
		utils.file.write(self.path, self.content.get_buffer())

	def __init_content(self) :
		self.content = utils.file_content()
		self.content.add_line(0, utils.const.generated_warning)
		self.content.add_line(0, "#include \"" + common_header.name + ".h\"")
		self.content.add_line(0, "using cacoon::comms::types;")

	def __del_content(self) :
		self.content.add_line(0, "") 

	def add(self, xml) :
		self.content.add_line(0, "const " + utils.const.namespace_type + utils.const.hash.type + " cacoon::comms::" + xml.get_name() + "::" + utils.const.hash.name + "[] = \"" + xml.get_hash() + "\";")

class message_factory :
	def __init__(self, dir) :
		self.name = "message_factory"
		self.path = dir + self.name + ".cxx"
		self.types = list()
		self.buffer = utils.file_content()
		self.buffer.add_line(0, "#include \"message_factory.h\"")
		self.buffer.add_line(0, "using cacoon::comms::types;")
		self.buffer.add_line(0, "using cacoon::comms::serializable;")
		self.buffer.add_line(0, "using cacoon::comms::message_factory;")
		self.buffer.add_line(0, "const std::map<const types::charU8*, message_factory::serializable_fn> m_map = {content};")

	def __del__(self) :
		types_len = len(self.types)
		if types_len == 0 :
			pairs_str = "{}"
		else :
			pairs_content = utils.file_content()
			pairs_content.add_line(0, "{")
			for i in range(types_len) :
				line = ""
				line +=  "{" + "cacoon::comms::{0}::id".format(self.types[i].get_name()) + ", {" + "&cacoon::comms::{0}::serialize, &cacoon::comms::{0}::deserialize".format(self.types[i].get_name()) + "}}"
				if i == 0 and types_len > 1:
					line += ","
				pairs_content.add_line(1, line)
			pairs_str = pairs_content.get_buffer() + "}"
		utils.file.write(self.path, self.buffer.get_buffer().format(content=pairs_str))

	def add(self, xml) :
		self.types.append(xml)

class messages_builder() :
	def __init__(self, outdir) :
		self.dir = outdir

	def build(self) :
		file_list = os.listdir(self.dir)
		common_h = common_header(self.dir)
		common_cpp = common_impl(self.dir)
		msg_factory = message_factory(self.dir)

		xml = None
		for file in file_list :
			if(".xml" in file) :  
				utils.print.info("Processing " + file)
				xml = xml_message(self.dir + file)
				xml.parse()
				utils.file.write_header(self.dir, xml)
				common_h.add(xml)
				common_cpp.add(xml)
				msg_factory.add(xml)

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("directory", help="directory containing messages metadata (xml)")
args = parser.parse_args()

if(args.directory) :
	#dir = "../libs/comms/messaging/messages_definitions/"
	builder = messages_builder(args.directory + "/")
	builder.build()
else :
	parse.print_help()