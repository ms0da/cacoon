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
		self.hash = utils.digest.sha256(utils.digest.sha256(id) + utils.digest.sha256(type))

import os, hashlib, time
class utils :
	class extension :
		def get_header() :
			return ".generated.h"
		def get_source() :
			return ".generated.cpp"
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
		def delete(path) :
			os.remove(path)

		def write_header(outdir, xml) :
			define = xml.get_name().upper() + "_GENERATED_H"

			content = utils.file_content()
			content.add_line("#ifndef " + define)
			content.add_line("#define " + define)
			content.add_line(utils.const.generated_warning)
			content.add_line("#include <cacoon/message.h>")
			content.add_line("namespace cacoon {")
			content.tab_inc()
			content.add_line("namespace comms {")
			content.tab_inc()
			content.add_line("namespace messages {")
			content.tab_inc()
			content.add_line("struct " + xml.get_name() + " : public serializable {")
			content.tab_inc()
			content.add_line("static const " + utils.const.namespace_type + utils.const.hash.type + " " + utils.const.hash.name + ";")
			
			attribs = xml.get_attributes();
			for attrib in attribs :
				comments = ""
				if "" != attrib.comments :
					content.add_line("// " + attrib.comments)
				content.add_line(attrib.type + " " + attrib.id + ";")

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

			content.add_line(xml.get_name() + "({}) ".format(constructor_params))
			content.add_line(" : {}".format(constructor_init) + " { }")
			content.add_line("static std::shared_ptr<serializable> deserialize(std::istream& is) {")
			content.tab_inc()
			content.add_line("std::shared_ptr<" + xml.get_name() + "> ptr;")
			content.add_line(deserialize_vars + ";")
			content.add_line("if(" + deserialize_if + ") {")
			content.tab_inc()
			content.add_line("ptr = std::make_shared<" + xml.get_name() + ">(" + shared_ptr + ");")
			content.tab_dec()
			content.add_line("}")
			content.add_line("return ptr;")
			content.tab_dec()
			content.add_line("}")
			content.tab_dec()
			content.add_line("private:")
			content.tab_inc()
			content.add_line("virtual void serialize_type(std::ostream& os) const override {")
			content.tab_inc()
			content.add_line("serialize_id(os, id);")
			attrs = "os "
			for attrib in xml.get_attributes() :
				attrs += " << '|' << " + attrib.id
			content.add_line(attrs + ";")
			content.tab_dec()
			content.add_line("}")
			content.tab_dec()
			content.add_line("};")
			content.tab_dec()
			content.add_line("}")
			content.tab_dec()
			content.add_line("}")
			content.tab_dec()
			content.add_line("}")
			content.add_line("#endif")
			utils.file.write(outdir + xml.get_name() + utils.extension.get_header(), content.get_buffer())

	class file_content :
		def __init__(self) :
			self.buffer = ""
			self.tabs = 0
		def tab_inc(self) :
			self.tabs += 1
		def tab_dec(self) :
			if self.tabs > 0 :
				self.tabs -= 1
		def add_tab(self, count) :
			for i in range(count) :
				self.buffer += "\t"
		def add_line(self, content = "") :
			for i in range(self.tabs) :
				self.buffer += "\t"
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
		namespace_type = "serializable::"
		class hash :
			type = "id_type"
			type_size = 8
			name = "id"
			array_len = 8

class common_header :
	name = "messages_types_includes"
	def __init__(self, outdir) :
		self.path = outdir + common_header.name + utils.extension.get_header()
		self.__init_content()
	def __del__(self) :
		self.__del_content()
		utils.file.write(self.path, self.content)

	def __init_content(self) :
		self.content = "#ifndef " + common_header.name.upper() + "_GENERATED_H\n"
		self.content += "#define " + common_header.name.upper() + "_GENERATED_H\n"
		self.content += utils.const.generated_warning + "\n"
	def __del_content(self) :
		self.content += "#endif\n"

	def add(self, xml) :
		self.content += "#include \"" + xml.get_name() + utils.extension.get_header() + "\"\n"

class common_impl :
	def __init__(self, dir) :
		self.name = "messages_types_implementation"
		self.path = dir + self.name + utils.extension.get_source()
		self.__init_content()
	def __del__(self) :
		self.__del_content()
		utils.file.write(self.path, self.content.get_buffer())

	def __init_content(self) :
		self.content = utils.file_content()
		self.content.add_line(utils.const.generated_warning)
		self.content.add_line("#include <serializable.h>")
		self.content.add_line("#include \"" + common_header.name + utils.extension.get_header() + "\"")
		self.content.add_line("using cacoon::comms::serializable;")

	def __del_content(self) :
		self.content.add_line("") 

	def add(self, xml) :
		self.content.add_line("const " + utils.const.namespace_type + utils.const.hash.type + " cacoon::comms::messages::" + xml.get_name() + "::" + utils.const.hash.name + " = {")
		self.content.tab_inc()
		start = 0
		end = utils.const.hash.type_size
		for i in range(utils.const.hash.array_len) :
			line = "0x" + xml.get_hash()[start:end]
			if i != utils.const.hash.array_len - 1 :
				line += ","
			self.content.add_line(line)
			start += utils.const.hash.type_size
			end += utils.const.hash.type_size
		self.content.tab_dec()
		self.content.add_line("};")

class message_factory :
	def __init__(self, dir) :
		self.name = "message_factory"
		self.path = dir + self.name + utils.extension.get_source()
		self.types = list()
		self.buffer = utils.file_content()
		self.buffer.add_line("#include \"message_factory.h\"")
		self.buffer.add_line("using cacoon::comms::types;")
		self.buffer.add_line("using cacoon::comms::serializable;")
		self.buffer.add_line("using cacoon::comms::message_factory;")
		self.buffer.add_line("const message_factory::container_type message_factory::m_map = {content};")

	def __del__(self) :
		types_len = len(self.types)
		if types_len == 0 :
			pairs_str = "{}"
		else :
			pairs_content = utils.file_content()
			pairs_content.add_line("{")
			pairs_content.tab_inc()
			for i in range(types_len) :
				line = ""
				line +=  "{message_factory::key_type(" + "cacoon::comms::messages::{0}::id".format(self.types[i].get_name()) + "), {" + "&cacoon::comms::messages::{0}::serialize, &cacoon::comms::messages::{0}::deserialize".format(self.types[i].get_name()) + "}}"
				if i == 0 and types_len > 1:
					line += ","
				pairs_content.add_line(line)
			pairs_str = pairs_content.get_buffer() + "}"
		utils.file.write(self.path, self.buffer.get_buffer().format(content=pairs_str))

	def add(self, xml) :
		self.types.append(xml)

class messages_builder() :
	def __init__(self, msg_dir_in, msg_dir_out) :
		self.msg_dir_in = msg_dir_in
		self.msg_dir_out = msg_dir_out

	def build(self) :
		file_list = os.listdir(self.msg_dir_in)
		if not os.path.exists(self.msg_dir_out) :
			os.makedirs(self.msg_dir_out)
		common_h = common_header(self.msg_dir_out)
		common_cpp = common_impl(self.msg_dir_out)
		msg_factory = message_factory(self.msg_dir_out)

		xml = None
		for file in file_list :
			if(".xml" in file) :  
				utils.print.info("Processing " + file)
				xml = xml_message(self.msg_dir_in + file)
				xml.parse()
				utils.file.write_header(self.msg_dir_out, xml)
				common_h.add(xml)
				common_cpp.add(xml)
				msg_factory.add(xml)

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("msg_dir_in", help="input directory containing messages metadata")
parser.add_argument("msg_dir_out", help="output directory for generated messages")
args = parser.parse_args()

if (args.msg_dir_in is not None) and (args.msg_dir_out is not None) :
	builder = messages_builder(args.msg_dir_in + "/", args.msg_dir_out + "/")
	builder.build()
else :
	parse.print_help()
