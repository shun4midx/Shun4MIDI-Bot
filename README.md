# Shun4MIDI-Bot
Future Discord Bot for the Shun4MIDI project, probably coded in C++ (Yes, I promised I would return to deving in June when the semester ends, I would keep my promise)

This repo is totally not to distance myself from the actual Shun4MIDI code (Only cool people like the you reading this would find it XD).

Yes, there is a possibility Shun4MIDI would be a Discord Bot before it is a website or a library (Probably a C++ library because I'm biased and there are so few libraries in C++ with something this cool), because I cannot get myself to code sometimes for the life of me but use Discord too much :|

Also, yes, this summer I am not only deving, I have other stuff to do (that are also frankly more interesting), so don't expect this to be done by the end of the summer holiday or whatever, but expect quite a few functionalities in place?

## Details
This is dedicated to the 2021-2023 Shun who still used to [compose Music](https://www.youtube.com/watch?v=fNU0zx5wI3Q) and wanted a more advanced Music Theory tool to help him without needing to do the theory by hand or in his head :) I don't really compose anymore since I don't have the time but I love composing still, at heart. :)

Priority would be placed on Jazz theory and more "obscure" theory such as quartal harmony, whole-tone harmony, etc first since that is my niche in music, although there would definitely be basic chords and scales coded before those are in place and those are quite universal. I am a Jazz-trained musical composer(*), not classical-trained, so it may affect my view on this project!

<i>(*) Ex-composer? I didn't quit officially but I didn't compose in one to two years ever since the first winter holiday ended after I entered college... I'm so busy... I don't even have much time to code this, let alone literally compose.</i>

Like how Math or CS has more theory based people, I consider myself to be *very* theoretical in terms of my passion in Music and Musical composition, so it may not have much purpose for your average contemporary composer? I like to compose in these niche genres (Not talking about Jazz) because I like theory, and I would thus prioritize that first.

## Some explanation of the harmony functions I plan to include copied from the original "official" repo 
### (I was too busy this semester to even work on the Japanese pentatonic scales and I kind of forgot about the rough musical idea... If I understand the notes I wrote about it like half a year ago I would try to implement it but I'm not sure. I may attach some handwritten notes on them here first if I don't have the time to code it before the summer holiday ends)

Alternate Harmony:
 - Support for quartal harmony 😎 (Quartal chord generation based on a root note, any inversion of quartal chords, modal interchange of quartal chords, "Given a (custom) scale or short musical excerpt, how many quartal chords can I generate with notes purely diatonic to the scale excerpt?", etc)
 - Support for whole-tone harmony 😍 (Whole-tone chord generation based on a root note, any inversion of whole-tone chords, modal interchange from other modes by weaving in whole-tone harmony if desired, etc)
 - Support for Japanese pentatonic scales (Yō-sen/陽旋 or In-sen/陰旋 as selected by the user). More will be revealed when the code is done, but let's just say it blends really well with quartal harmony as a new hybrid of "J-Pentatonic Harmony" (coined by me, 2025), if I can get it worked out, which I'm 90% sure I can 👀 The main idea of what I will offer is modulation between any Yō-sen and In-sen scale, and also some things that are related to hybrid and quartal harmony in these scales (which I call J-Pentatonic Harmony), that I can't yet reveal, since they are relatively new concepts that I don't doubt I may be one of the first to actually document.
 - (*) Support for the inputing of custom scales/solfège for analysis, although for the C++ code it will only last for one "Run" attempt and reset afterwards. As of right now, I only support the inputing into a Western-notation equivalent. It will support microtonal notation too. I'm not diving into microtonal harmony at least for the Shun4MIDI Music Theory Tool, but this would be only natural to preserve authenticity of custom scales. There will be an option to include a different rising and falling scale! Transposition of these scales to different starting keys are supported, and visiual representation is offered. When inputed a melody in solfège of a custom scale, basic suggestions with vibrato and gliding will be supported.

(*) This is because, "Western music theory" isn't the only music theory there is out there, and this is a way to say "I'm not so familiar with other music theory, but this is my way of supporting e.g. Indian ragas and Arabic maqam". There would not be harmonic analysis, since these musical genres do not center around harmony.

*Can you tell quartal harmony and whole-tone melody were a huge hyperfixation of mine in high school? :D Yō-sen and In-sen were fleeting interests of mine too so I loved to see the connections between these forms of music!*

## Remark on the Inclusion of Indian ragas and Arabic maqam
Something to add in June 2025 especially because of the current state of politics: No this is not me being woke, read a goddamn Music history book that isn't centered around European Music before you tell me otherwise. The least I can do as someone who is Asian is to not think European Music is the only form of Music, and actually learn Music theory of other cultures.

## Remark II
I would support microtonal notation like half-sharps and half-flats, don't mistake that. However, by me saying I won't dive into "microtonal harmony", I moreso mean harmony that isn't the typical major/minor/12-TET inspired harmony. For example, what is it like to not only consider half steps but for example, quarter steps between notes in a chord? Yes, as a more Math-ey person, I used to be **very obsessed** with microtonal composition, and have thus read up on microtonal theory (and composed some microtonal pieces produced on Logic Pro too :>). Trust me, there is a difference between supporting microtonal notes with 12-TET inspired harmony adapted to microtonality and actually having microtonal harmony. If you don't trust me, look at Jacob Collier's music 😂. I am a *bit* more rusty about microtonal harmony now that I haven't done it in more than two years, but it is an interest of mine. Feel free to talk to me more about it if you want.