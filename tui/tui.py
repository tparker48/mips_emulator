# Absolute pile of slop (rough demo)

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


class Header(Header):
    pass

class ToolbarButton(Button):
    DEFAULT_CSS = """
    ToolbarButton {
        height: 1;
    }
    """

class Toolbar(HorizontalGroup):
    DEFAULT_CSS = """
    Toobar {
        height: 1;
        dock: top;
    }
    """
    def compose(self) -> ComposeResult:
        yield ToolbarButton("File", id="File", compact=True)
        yield ToolbarButton("Assemble", compact=True)
        yield ToolbarButton("Run", compact=True)

class Footer(Footer):
    pass

class CodeArea(TextArea):
    DEFAULT_CSS = """
    CodeArea {
        width: 1fr;
        height: 1fr;
    }
    """

tabs = [
    ["test1", CodeArea(soft_wrap=False)],
    ["test2", CodeArea(soft_wrap=False)]
]


class CodeWindow(TabbedContent):
    DEFAULT_CSS = """
    CodeWindow {
        width: 1fr;
        height: 1fr;
    }
    """
    def compose(self) -> ComposeResult:
        with TabbedContent():
            for name, widget in tabs:
                with TabPane(name):
                    yield widget

class LogWindow(Log):
    DEFAULT_CSS = """
    LogWindow {
        width: 1fr;
        height: 1fr;
        scrollbar-visibility: hidden;
    }
    """

class DirectoryTree(DirectoryTree):
    DEFAULT_CSS = """
    DirectoryTree {
        width: 0.5fr;
        height: 1fr;
    }
    """

class ColumnsContainer(VerticalGroup):
    DEFAULT_CSS = """
    ColumnsContainer {
        width: 1fr;
        height: 1fr;
    }
    """
    def compose(self) -> ComposeResult:
        # yield DirectoryTree("./")
        yield CodeArea(id="Code")
        yield LogWindow(id="Log", auto_scroll=True)
        
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

class SomeScreen(Screen):
    def compose(self) -> ComposeResult:
        #yield Header(id="Header")
        yield Toolbar(id="Toolbar")
        yield Footer(id="Footer")
        yield ColumnsContainer(id="Columns")
        
    def on_button_pressed(self, event: Button.Pressed) -> None:
        if event.button.id == "File":
            self.app.push_screen(OverlayScreen())


class MIPSEmulator(App):
    def on_mount(self) -> None:
        self.push_screen(OverlayScreen())
        self.push_screen(SomeScreen())


if __name__ == "__main__":
    app = MIPSEmulator()
    app.run()