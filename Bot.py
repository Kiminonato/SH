import os
import discord
from discord.ext import commands
import firebase_admin
from firebase_admin import credentials, db

cred = credentials.Certificate("firebase-key.json")
firebase_admin.initialize_app(cred,
                              {'databaseURL': os.environ['FIREBASE_DB_URL']})

intents = discord.Intents.default()
intents.message_content = True
bot = commands.Bot(command_prefix='!', intents=intents)


@bot.event
async def on_ready():
    print(f':robot: บอทออนไลน์ในชื่อ {bot.user}')


@bot.command(name='โรงรถปิด')
async def fix_servo(ctx):

    ref = db.reference("/servo")
    ref.set({'angle': 180})

    #await asyncio.sleep(2)  # รอ 2 วินาที
    await ctx.send("ประตูโรงรถปิด!")


@bot.command(name='โรงรถเปิด')
async def fix_servo(ctx):

    ref = db.reference("/servo")
    ref.set({'angle': 0})

    #await asyncio.sleep(2)  # รอ 2 วินาที
    await ctx.send("ประตูโรงรถเปิด!")


@bot.command(name='servo')
async def servo(ctx, angle: int):
    if angle < 0 or angle > 180:
        await ctx.send("กรุณาใส่ค่าระหว่าง 0 ถึง 180")
        return

    ref = db.reference("/servo")
    ref.set({'angle': angle})
    await ctx.send(f"ตั้ง servo ไปที่ {angle} องศาแล้ว!")


@bot.command(name='เปิดไฟห้องนอน')
async def led_on(ctx):
    ref = db.reference("/led")
    ref.set({'state': 1})
    await ctx.send(":bulb: เปิดไฟแล้ว!")


@bot.command(name='ปิดไฟห้องนอน')
async def led_off(ctx):
    ref = db.reference("/led")
    ref.set({'state': 0})
    await ctx.send(":bulb: ปิดไฟแล้ว!")


bot.run(os.environ['DISCORD_BOT_TOKEN'])
