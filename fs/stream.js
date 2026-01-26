function* randomGenerator() 
{
	var count = 0;
    while (true) 
    {
		console.log(count++);
        yield Math.random();
    }
}

function getNextRandom(gen) 
{
    return new Promise(resolve => { setTimeout(() => { resolve(gen.next().value); }, 500); });
}

const gen = randomGenerator();

async function streamRandomData(count = 10) 
{
    for (let i = 0; i < count; i++) 
    {
        const value = await getNextRandom(gen);
        console.log(value);
    }
}

streamRandomData();

