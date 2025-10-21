local speed = 0.18

local function setup(entity) end

local function update(entity)
	if getButtonHeld("a") then
		entity.angle = entity.angle - speed
	end
	if getButtonHeld("d") then
		entity.angle = entity.angle + speed
	end
	if getButtonHeld("w") then
		moveForward(entity, speed)
	end
end

return {
	setup = setup,
	update = update,
}
