import math, random, sys
from PIL import Image, ImageDraw, ImageFont
S=4; N=64*S
OUT=sys.argv[1]
BLK=(20,20,20,255); BLUE=(110,165,225,255); LBLUE=(200,225,250,255); RED=(225,50,50,255); LRED=(240,120,120,255); GRN=(60,160,80,255); GRAY=(150,150,150,255)
def canvas(): im=Image.new('RGBA',(N,N),(0,0,0,0)); return im, ImageDraw.Draw(im)
def P(*xy): return [v*S for v in xy]
def save(im,name): im.resize((64,64),Image.LANCZOS).save(f'{OUT}/{name}.png'); print(name)
def font(sz):
    for f in ['/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf','/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf']:
        try: return ImageFont.truetype(f,sz*S)
        except: pass
    return ImageFont.load_default()

# save_as: floppy + pencil
im,d=canvas()
d.rounded_rectangle(P(6,6,46,46),radius=3*S,fill=BLUE,outline=BLK,width=2*S)
d.rectangle(P(14,6,38,20),fill=LBLUE,outline=BLK,width=2*S); d.rectangle(P(31,9,35,17),fill=BLK)
d.rectangle(P(12,28,40,46),fill=(250,250,250,255),outline=BLK,width=2*S)
for y in (33,38,43): d.line(P(16,y,36,y),fill=GRAY,width=S)
pen=[(58,30),(62,34),(38,58),(32,60),(34,54)]
d.polygon([(x*S,y*S) for x,y in pen],fill=(245,200,60,255),outline=BLK); d.line(P(58,30,62,34,38,58,32,60,34,54,58,30),fill=BLK,width=2*S)
d.polygon([(x*S,y*S) for x,y in [(32,60),(34,54),(38,58)]],fill=BLK)
save(im,'save_as')

# exit: door + arrow
im,d=canvas()
d.rectangle(P(8,6,34,58),fill=LBLUE,outline=BLK,width=2*S)
d.polygon([(x*S,y*S) for x,y in [(8,6),(26,12),(26,62),(8,58)]],fill=BLUE,outline=BLK,width=2*S)
d.ellipse(P(20,32,24,36),fill=BLK)
d.line(P(34,32,58,32),fill=RED,width=5*S); d.polygon([(x*S,y*S) for x,y in [(60,32),(48,22),(48,42)]],fill=RED)
save(im,'exit')

# about: blue circle with i
im,d=canvas()
d.ellipse(P(6,6,58,58),fill=BLUE,outline=BLK,width=2*S)
d.ellipse(P(28,14,36,22),fill='white'); d.rounded_rectangle(P(28,27,36,50),radius=S,fill='white')
save(im,'about')

# bilateral filter: noisy surface -> smooth surface
im,d=canvas(); random.seed(3)
noisy=[(x, 22+6*math.sin(x/9)+random.uniform(-5,5)) for x in range(4,61,4)]
smooth=[(x, 48+6*math.sin(x/9)) for x in range(4,61,2)]
d.polygon([(x*S,y*S) for x,y in noisy]+[(60*S,30*S),(4*S,30*S)] ,fill=LRED)
d.line([(x*S,y*S) for x,y in noisy],fill=RED,width=2*S)
for x,y in noisy: d.ellipse(P(x-1.8,y-1.8,x+1.8,y+1.8),fill=BLK)
d.polygon([(x*S,y*S) for x,y in smooth]+[(60*S,60*S),(4*S,60*S)],fill=LBLUE)
d.line([(x*S,y*S) for x,y in smooth],fill=BLUE,width=3*S)
d.polygon([(x*S,y*S) for x,y in [(32,33),(26,28),(38,28)]],fill=BLK)
save(im,'bilateral_filter')

# recon_hoppe: points with tangent planes -> implicit surface
im,d=canvas()
curve=[(x, 40-14*math.sin((x-6)/52*math.pi)) for x in range(6,59)]
d.polygon([(x*S,y*S) for x,y in curve]+[(58*S,58*S),(6*S,58*S)],fill=LBLUE)
d.line([(x*S,y*S) for x,y in curve],fill=BLUE,width=3*S)
for x in (12,22,32,42,52):
    y=40-14*math.sin((x-6)/52*math.pi); dy=-14*math.cos((x-6)/52*math.pi)*math.pi/52
    n=math.hypot(1,dy); tx,ty=1/n,dy/n
    d.line(P(x-6*tx,y-6*ty,x+6*tx,y+6*ty),fill=GRN,width=2*S)
    d.line(P(x,y,x+9*ty,y-9*tx),fill=RED,width=2*S)
    d.ellipse(P(x-2.5,y-2.5,x+2.5,y+2.5),fill=BLK)
save(im,'recon_hoppe')

# recon_sdf: signed distance grid with zero level set
im,d=canvas(); c=(32,32); R=17
for i in range(6):
    for j in range(6):
        x0,y0=5+i*9,5+j*9; cx,cy=x0+4.5,y0+4.5
        sd=math.hypot(cx-c[0],cy-c[1])-R; t=max(-1,min(1,sd/20))
        col=(int(255-100*max(t,0)),int(255-120*abs(t)),int(255-100*max(-t,0)),255) if False else ((240,int(170+80*(1-t)),int(170+80*(1-t)),255) if t>0 else (int(170+80*(1+t)),int(200+50*(1+t)),250,255))
        d.rectangle(P(x0,y0,x0+9,y0+9),fill=col,outline=(120,120,120,255),width=S)
d.ellipse(P(c[0]-R,c[1]-R,c[0]+R,c[1]+R),outline=BLK,width=3*S)
f=font(9); d.text((c[0]*S,c[1]*S),"−",font=f,fill=BLUE,anchor='mm'); d.text((54*S,10*S),"+",font=f,fill=RED,anchor='mm')
save(im,'recon_sdf')

# deformation: embedded deformation graph on a bent mesh + drag arrow
im,d=canvas()
rows=[[(x, 44-18*math.sin(x/60*math.pi)*(x/60)+r*8) for x in range(6,59,13)] for r in (0,1)]
for r in range(1):
    for k in range(len(rows[0])-1):
        a,b,c2,e=rows[0][k],rows[0][k+1],rows[1][k],rows[1][k+1]
        d.polygon([(p[0]*S,p[1]*S) for p in (a,b,e,c2)],fill=LBLUE,outline=BLK)
        d.line([(a[0]*S,a[1]*S),(e[0]*S,e[1]*S)],fill=BLK,width=S)
for row in rows: d.line([(x*S,y*S) for x,y in row],fill=BLK,width=2*S)
nodes=[rows[0][0],rows[0][2],rows[0][4]]
d.line([(x*S,(y-12)*S) for x,y in nodes],fill=GRN,width=2*S)
for x,y in nodes: d.line(P(x,y,x,y-12),fill=GRAY,width=S); d.ellipse(P(x-4,y-16,x+4,y-8),fill=GRN,outline=BLK,width=S)
d.line(P(40,14,54,6),fill=RED,width=3*S); d.polygon([(x*S,y*S) for x,y in [(58,4),(48,5),(53,13)]],fill=RED)
save(im,'deformation')

# kinect: depth sensor bar
im,d=canvas()
d.polygon([(x*S,y*S) for x,y in [(28,40),(36,40),(40,56),(24,56)]],fill=(60,60,60,255),outline=BLK)
d.rounded_rectangle(P(4,22,60,40),radius=6*S,fill=(45,45,45,255),outline=BLK,width=2*S)
d.rounded_rectangle(P(4,22,60,28),radius=4*S,fill=(80,80,80,255))
for cx,col in ((16,(90,90,90,255)),(32,(40,90,200,255)),(48,(90,90,90,255))):
    d.ellipse(P(cx-5,26,cx+5,36),fill=col,outline=(10,10,10,255),width=S)
d.ellipse(P(30,29,34,33),fill=(160,200,255,255))
d.ellipse(P(54,30,57,33),fill=GRN)
save(im,'kinect')
