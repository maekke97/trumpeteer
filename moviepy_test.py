#!/usr/bin/env python
from moviepy.editor import *
from gtts import gTTS
from aeneas.tools.execute_task import ExecuteTaskCLI
import os
import json


def generate_mp3(text, temp_audio='tmp.mp3', accent='en'):
    tts = gTTS(text=text, lang=accent)
    with open(temp_audio, 'wb') as of:
        tts.write_to_fp(of)

        
def analyze_audio(text, temp_audio='tmp.mp3', sync_map='word_times.json', temp_text='tmp.txt'):
    with open(temp_text, 'w') as of:
        of.write(text.replace(' ', '\n'))
              
    ExecuteTaskCLI(use_sys=False).run(arguments=[
        None,  # dummy program name argument
        temp_audio,
        temp_text,
        u"task_language=eng|is_text_type=plain|os_task_file_format=json",
        sync_map])
    os.remove(temp_text)
              

def generate_mp4(sync_map='word_times.json', out_file='myvideo.mp4', temp_audio='tmp.mp3',
                 size=(1200, 800), screen_length=20):
    clips = []
    text = ''
    line_length = 0
    with open(sync_map, 'r') as infile:
        info = json.load(infile)
    for record in info['fragments']:
        word = record['lines'][0]
        start = float(record['begin'])
        end = float(record['end'])
        length = end - start
        text += word
        line_length += len(word)
        if line_length > screen_length:
            text += '\n'
            line_length = 0
        else:
            text += ' '
            line_length += 1
        current_clip = TextClip(text, font='Amiri-Bold', fontsize=60, size=size,
                                stroke_width=0, bg_color='black', color='white')
        current_clip.duration = length
        clips.append(current_clip)

    my_audio = AudioFileClip(temp_audio)
    cvc = concatenate_videoclips(clips)
    cvc.audio = my_audio

    cvc.write_videofile(out_file, fps=25)
    os.remove(temp_audio)
              

if __name__ == '__main__':
    my_text = "The ABC/Washington Post Poll, even though almost 40% is not bad at this time, was just about the most " \
           "inaccurate poll around election time! "
    generate_mp3(my_text)
    analyze_audio(my_text)
    generate_mp4()
