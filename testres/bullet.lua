local speed = 500

local function setup(entity) end

local function update(entity, deltaTime)
	moveAtAngle(entity, speed * deltaTime, entity.props["direction"])

	local zombies = getEntitiesByDef("Zombie")
	for i = 1, zombies:size() do
		if isColliding(entity, zombies[i]) then
			kill(zombies[i].id)
			kill(entity.id)
			break
		end
	end
end

return {
	setup = setup,
	update = update,
}
