local L = {}

L.lexer = "yaml"

L.singleLineComment = "# "

L.extensions = {
	"yml",
	"yaml",
}

L.keywords = {
	[0] = "true false yes no",
}

L.styles = {
	["DEFAULT"] = {
		id = 0,
		fgColor = darkFg,
		bgColor = darkBg,
	},
	["IDENTIFIER"] = {
		id = 2,
		fgColor = rgb(0x000080),
		bgColor = darkBg,
		fontStyle = 1,
	},
	["COMMENT"] = {
		id = 1,
		fgColor = rgb(0x008000),
		bgColor = darkBg,
	},
	["INSTRUCTION WORD"] = {
		id = 3,
		fgColor = rgb(0x0000FF),
		bgColor = darkBg,
		fontStyle = 1,
	},
	["NUMBER"] = {
		id = 4,
		fgColor = rgb(0xFF8040),
		bgColor = darkBg,
	},
	["REFERENCE"] = {
		id = 5,
		fgColor = rgb(0x804000),
		bgColor = darkBg,
	},
	["DOCUMENT"] = {
		id = 6,
		fgColor = rgb(0x0000FF),
		bgColor = darkBg,
	},
	["TEXT"] = {
		id = 7,
		fgColor = rgb(0x808080),
		bgColor = darkBg,
		fontStyle = 1,
	},
	["ERROR"] = {
		id = 8,
		fgColor = rgb(0xFF0000),
		bgColor = darkBg,
	},
}
return L
