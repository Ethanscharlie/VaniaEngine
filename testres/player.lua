local speed = 0.1

local function setup(entity)
	entity.x = entity.x + 100

	summon("Zombie", 10, 10)
end

local function update(entity) end

local function keyHeldRight(entity)
	entity.x = entity.x + speed
end

local function keyHeldLeft(entity)
	entity.x = entity.x - speed
end

local function keyHeldUp(entity)
	entity.y = entity.y - speed
end

local function keyHeldDown(entity)
	entity.y = entity.y + speed
end

local function keyHeldSpace(entity)
	entity.x = entity.x - speed / 2
	entity.y = entity.y - speed / 2
	entity.def.width = entity.def.width + speed
	entity.def.height = entity.def.height + speed
end

return {
	setup = setup,
	update = update,
	keyHeldRight = keyHeldRight,
	keyHeldLeft = keyHeldLeft,
	keyHeldUp = keyHeldUp,
	keyHeldDown = keyHeldDown,
	keyHeldSpace = keyHeldSpace,
}
