local speed = 100

local function setup(entity)
	entity.props["cooldown"] = 0
	entity.props.player = getEntitiesByDef("Player")[1]
end

local function update(entity, deltaTime)
	lookAt(entity, entity.props.player.x, entity.props.player.y)
	moveForward(entity, speed * deltaTime)
end

return {
	setup = setup,
	update = update,
}
