local speed = 500

local function setup(entity) end

local function update(entity, deltaTime)
	moveForward(entity, speed * deltaTime)
end

return {
	setup = setup,
	update = update,
}
