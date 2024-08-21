
import asyncio

async def fn1():
        print ("one")
        print ("two")
        await asyncio.sleep(1)
        print ("three")

async def fn2():
        print ("four")
        await asyncio.sleep(1)
        print ("five")

async def main():
        await asyncio.gather(fn1(),fn2())

asyncio.run(main())
