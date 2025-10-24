local speed = 300
local cooldownTime = 0.3

local function setup(entity)
	entity.props["cooldown"] = 0
end

local function update(entity, deltaTime)
	local m = mousePos()
	lookAt(entity, m.x, m.y)

	if getButtonHeld("a") then
		entity.x = entity.x - speed * deltaTime
	end
	if getButtonHeld("d") then
		entity.x = entity.x + speed * deltaTime
	end
	if getButtonHeld("w") then
		entity.y = entity.y - speed * deltaTime
	end
	if getButtonHeld("s") then
		entity.y = entity.y + speed * deltaTime
	end

	local onCooldown = os.clock() < cooldownTime + entity.props["cooldown"]
	if getMouseButtonHeld("left") and not onCooldown then
		bullet = summon("Bullet", entity.x, entity.y)
		bullet.angle = entity.angle
		entity.props["cooldown"] = os.clock()
	end
end

return {
	setup = setup,
	update = update,
}
