# Absolute pile of slop (rough demo)

import os, sys
import subprocess
import time

from textual.app import App, ComposeResult
from textual.screen import Screen
from textual.widgets import Placeholder

from textual.widgets import OptionList
from textual.widgets.option_list import Option

from textual.widgets import Header
from textual.widgets import Footer
from textual.widgets import TextArea
from textual.widgets import Log
from textual.widgets import DirectoryTree

from textual.widgets import TabbedContent, TabPane

from textual.widgets import Button

from textual.containers import VerticalGroup
from textual.containers import HorizontalGroup

from assembler.assemble import assemble




stdout = sys.stdout
stderr = sys.stderr



class ToolbarButton(Button):
    DEFAULT_CSS = """
    ToolbarButton {
        height: 1;
    }
    """

class CodeToolbar(HorizontalGroup):
    DEFAULT_CSS = """
    Toobar {
        height: 1;
        dock: top;
    }
    """
    def compose(self) -> ComposeResult:
        #yield ToolbarButton("File", id="File", compact=True)
        yield ToolbarButton("Assemble", id="Assemble", compact=True, variant="default")
        yield ToolbarButton("Run", id="Run", compact=True, variant="primary")

class LogToolbar(HorizontalGroup):
    DEFAULT_CSS = """
    Toobar {
        height: 1;
        dock: top;
    }
    """
    def compose(self) -> ComposeResult:
        yield ToolbarButton("Clear", id="Clear", compact=True, variant="default")


class Footer(Footer):
    pass

class CodeWindow(TextArea):
    DEFAULT_CSS = """
    CodeArea {
        width: 1fr;
        height: 1fr;
    }
    """

class LogWindow(Log):
    DEFAULT_CSS = """
    LogWindow {
        width: 1fr;
        height: 1fr;
    }
    """
    pass

code = CodeWindow(id="Code", show_line_numbers=True, tab_behavior='indent')
log = LogWindow(id="Log", auto_scroll=True)

class ColumnsContainer(VerticalGroup):
    DEFAULT_CSS = """
    ColumnsContainer {
        width: 1fr;
        height: 1fr;
    }
    """
    def compose(self) -> ComposeResult:
        # yield DirectoryTree("./")
        yield CodeToolbar(id="CodeToolbar")
        yield code
        yield LogToolbar(id="LogToolbar")
        yield log
        
class OverlayScreen(Screen):
    def compose(self) -> ComposeResult:
        yield OptionList(
            Option("New", id="New"), 
            None,
            Option("Load", id="Load"), 
            None,
            Option("Save", id="Save"), 
            None,
            Option("Save As", id="Save As"), 
        )

    def on_option_list_option_selected(self, option: OptionList.OptionSelected) -> None:
        print(option.option_id)
        if option.option_id == "New":
            self.app.pop_screen()

    def _on_key(self, event):
        if event.key == "escape":
            self.app.pop_screen()
        return super()._on_key(event)

class MainScreen(Screen):
    def compose(self) -> ComposeResult:
        yield Header(id="Header")
        yield ColumnsContainer(id="Columns")
        yield Footer(id="Footer")
        
    def on_button_pressed(self, event: Button.Pressed) -> None:
        if event.button.id == "File":
            return
            self.app.push_screen(OverlayScreen())
        elif event.button.id == "Assemble":
            assemble_current_text()
        elif event.button.id == "Run":
            run_emulator()
        elif event.button.id == "Clear":
            log.clear()

def assemble_current_text():
    with open("tmp.asm", "w") as asm_file:
        asm_file.write(code.text)

    with open('assembler_output.log', 'w') as f_out:
        sys.stdout = f_out
        sys.stderr = f_out

        try:
            assemble("tmp.asm", "tmp.bin", verbose=True)
            print("Success")

        except Exception as e:
            print(e)
            print("Fail")

        
    sys.stdout = stdout
    sys.stderr = stderr

    with open('assembler_output.log', 'r') as f_out:
        log.write_lines(f_out.readlines())
    
    os.remove('assembler_output.log')

def run_emulator():
    log.write_line("Running...")

    with open('emulator_output.log', 'w') as f_out:
        subprocess.run('.\\emulator\\mips_sim.exe tmp.bin', stdout=f_out, stderr=f_out)

    with open('emulator_output.log', 'r') as f_out:
        log.write_lines(f_out.readlines())

    os.remove('emulator_output.log')


class MIPSEmulator(App):
    def on_mount(self) -> None:
        self.theme = "tokyo-night"
        self.push_screen(MainScreen())

if __name__ == "__main__":
    app = MIPSEmulator()
    app.run()