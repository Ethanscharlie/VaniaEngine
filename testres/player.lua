local speed = 300
local cooldownTime = 0.3

local function setup(entity)
	entity.props["cooldown"] = 0
end

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

	local onCooldown = os.clock() < cooldownTime + entity.props["cooldown"]
	if getButtonHeld("f") and not onCooldown then
		bullet = summon("Bullet", entity.x, entity.y)
		bullet.angle = entity.angle
		entity.props["cooldown"] = os.clock()
	end
end

return {
	setup = setup,
	update = update,
}
