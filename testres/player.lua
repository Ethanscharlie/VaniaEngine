local speed = 0.1

local function setup(entity) end

local function update(entity)
	if getButtonHeld("right") then
		entity.x = entity.x + speed
	end
	if getButtonHeld("left") then
		entity.x = entity.x - speed
	end
	if getButtonHeld("up") then
		entity.y = entity.y - speed
	end
	if getButtonHeld("down") then
		entity.y = entity.y + speed
	end
	if getButtonHeld(" ") then
		entity.x = entity.x - speed / 2
		entity.y = entity.y - speed / 2
		entity.def.width = entity.def.width + speed
		entity.def.height = entity.def.height + speed
	end
end

return {
	setup = setup,
	update = update,
}
