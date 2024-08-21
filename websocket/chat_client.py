import asyncio
import websockets

async def input_in_thread():
        loop = asyncio.get_running_loop()
        return await loop.run_in_executor(None, input,"Enter Message : ")

async def main():
        uri = "ws://192.168.188.143:8765"
        async with websockets.connect(uri) as websocket:
                print("Connect to Server")

                async def send_message():
                        while True:
                                message = await input_in_thread()
                                await websocket.send(message)
                async def recieve_message():
                        while True:
                                try:
                                        message = await websocket.recv()
                                        print(f"Recieve : {message}")
                                except websockets.exceptions.ConnectionClosed:
                                        print("Conection Closed")
                                        break
                await asyncio.gather(send_message(), recieve_message())

if __name__== "__main__":
        asyncio.run(main())
