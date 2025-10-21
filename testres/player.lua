local speed = 300

local function setup(entity) end

local function update(entity, deltaTime)
	if getButtonHeld("a") then
		entity.angle = entity.angle - speed * deltaTime
	end
	if getButtonHeld("d") then
		entity.angle = entity.angle + speed * deltaTime
	end
	if getButtonHeld("w") then
		moveForward(entity, speed * deltaTime)
	end
end

return {
	setup = setup,
	update = update,
}
