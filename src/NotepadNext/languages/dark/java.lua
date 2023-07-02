local L = {}

L.lexer = "cpp"

L.singleLineComment = "// "

L.extensions = {
	"java",
}

L.properties = {
	["fold.cpp.comment.explicit"] = "0",
	["lexer.cpp.track.preprocessor"] = "0",
	["lexer.cpp.escape.sequence"] = "1",
}

L.keywords = {
	[0] = "instanceof assert if else switch case default break goto return for while do continue new throw throws try catch finally this super extends implements import true false null",
	[1] = "package transient strictfp void char short int long double float const static volatile byte boolean class interface native private protected public final abstract synchronized enum",
	[5] = "FIXME TODO XXX HACK NOTE",
}

L.styles = {
	["WHITE SPACCE"] = {
		id = 0,
		fgColor = darkFg,
		bgColor = darkBg,
	},
	["PREPROCESSOR"] = {
		id = 9,
		fgColor = rgb(0x804000),
		bgColor = darkBg,
	},
	["DEFAULT"] = {
		id = 11,
		fgColor = darkFg,
		bgColor = darkBg,
	},
	["INSTRUCTION WORD"] = {
		id = 5,
		fgColor = rgb(0x0000FF),
		bgColor = darkBg,
		fontStyle = 1,
	},
	["TYPE WORD"] = {
		id = 16,
		fgColor = rgb(0x8000FF),
		bgColor = darkBg,
	},
	["NUMBER"] = {
		id = 4,
		fgColor = rgb(0xFF8000),
		bgColor = darkBg,
	},
	["STRING"] = {
		id = 6,
		fgColor = rgb(0x808080),
		bgColor = darkBg,
	},
	["CHARACTER"] = {
		id = 7,
		fgColor = rgb(0x808080),
		bgColor = darkBg,
	},
	["OPERATOR"] = {
		id = 10,
		fgColor = rgb(0x000080),
		bgColor = darkBg,
		fontStyle = 1,
	},
	["VERBATIM"] = {
		id = 13,
		fgColor = darkFg,
		bgColor = darkBg,
	},
	["REGEX"] = {
		id = 14,
		fgColor = darkFg,
		bgColor = darkBg,
		fontStyle = 1,
	},
	["COMMENT"] = {
		id = 1,
		fgColor = rgb(0x008000),
		bgColor = darkBg,
	},
	["COMMENT LINE"] = {
		id = 2,
		fgColor = rgb(0x008000),
		bgColor = darkBg,
	},
	["COMMENT DOC"] = {
		id = 3,
		fgColor = rgb(0x008080),
		bgColor = darkBg,
	},
	["COMMENT LINE DOC"] = {
		id = 15,
		fgColor = rgb(0x008080),
		bgColor = darkBg,
	},
	["COMMENT DOC KEYWORD"] = {
		id = 17,
		fgColor = rgb(0x008080),
		bgColor = darkBg,
		fontStyle = 1,
	},
	["COMMENT DOC KEYWORD ERROR"] = {
		id = 18,
		fgColor = rgb(0x008080),
		bgColor = darkBg,
	},
	["TASK MARKER"] = {
		id = 26,
		fgColor = rgb(0x008000),
		bgColor = darkBg,
		fontStyle = 1,
	},
}
return L
