
function ReduceHealth(amount)
	currentPlayerHealth = currentPlayerHealth - amount
	if currentPlayerHealth <= 0 then
		Quit()
	end
end