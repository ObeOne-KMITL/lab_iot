
import asyncio
import websockets

async def communicate():
        uri = "ws://192.168.188.143:8765"
        async with websockets.connect(uri) as websocket:
                message = "hello from client"
                print (f"Send message to server :{message}")
                await websocket.send(message)

                response = await websocket.recv()
                print (f"Recieve Response from server : {response}")

if __name__== "__main__":
        asyncio.run(communicate())
