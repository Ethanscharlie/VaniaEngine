local function setup(entity) end

local function update(entity, deltaTime)
	local m = mousePos()
	entity.x = m.x
	entity.y = m.y

	entity.def.b = 255
	entity.def.r = 255

	local rects = getEntitiesByDef("TestingRect")
	local size = rects:size()
	for i = 1, size do
		if isColliding(entity, rects[i]) then
			entity.def.b = 0
			entity.def.r = 0
		end
	end
end

return {
	setup = setup,
	update = update,
}
