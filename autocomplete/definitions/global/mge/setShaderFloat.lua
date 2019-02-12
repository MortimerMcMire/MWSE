return {
	type = "function",
	description = [[Sets a shader's float variable. Wrapper for mwscript's MGEShaderSetFloat.]],
	arguments = {
		{ name = "shader", type = "string" },
		{ name = "variable", type = "string" },
		{ name = "value", type = "number" },
	},
}