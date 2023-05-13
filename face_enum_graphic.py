from pathlib import Path

from face_enumeration import Interface_KP

from graphviz import Digraph


class FaceEnum():
    def __init__(self, path_to_file: Path | None):
        if path_to_file:
            vector_interpretation = self.read_from_file(path_to_file)
        else:
            vector_interpretation = []
        self.enumeration_interface = Interface_KP(vector_interpretation)
        self.enumeration_interface.ConvertToData()
        self.is_enumerated = False

    def read_from_file(self, path: Path) -> list:
        result_data_vector = []
        with open(path, 'r') as fl:
            lines = fl.readlines()
            for line in lines:
                result_data_vector.append(list(map(lambda x: int(x),line.split())))
        return result_data_vector

    def face_enumeration(self):
        """
        Run C++ implementation of face enumeration
        """
        self.enumeration_interface.FindAllFace()
        self.is_enumerated = True

    def print_text_hasse_diagramm(self):
        """
        Print Hasse diagramm to console in text form
        """
        if self.is_enumerated:
            self.enumeration_interface.Output()

    def draw_hasse_diagram(self):
        list_enterpretation = self.enumeration_interface.GraphPostProcessing()
        graph_interpret = Digraph()
        for pair in list_enterpretation:
            graph_interpret.edge(pair[0], pair[1])


if __name__ == "__main__":
    test_face_enum = FaceEnum(Path.cwd() / 'Hasse_diagram' / 'examples' / 'sqr.txt')
    test_face_enum.face_enumeration()
    test_face_enum.draw_hasse_diagram()