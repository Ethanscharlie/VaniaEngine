local speed = 0.1

function setup(entity)
	entity.x = entity.x + 100
end

function update(entity) end

function keyHeldRight(entity)
	entity.x = entity.x + speed
end

function keyHeldLeft(entity)
	entity.x = entity.x - speed
end

function keyHeldUp(entity)
	entity.y = entity.y - speed
end

function keyHeldDown(entity)
	entity.y = entity.y + speed
end

function keyHeldSpace(entity)
	entity.x = entity.x - speed / 2
	entity.y = entity.y - speed / 2
	entity.def.width = entity.def.width + speed
	entity.def.height = entity.def.height + speed
end
