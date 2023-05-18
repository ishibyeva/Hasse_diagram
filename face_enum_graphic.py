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

    @staticmethod
    def read_from_file(path: Path) -> list[int]:
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
        if not self.is_enumerated:
            self.enumeration_interface.FindAllFace()
            self.is_enumerated = True

    def print_text_hasse_diagramm(self):
        """
        Print Hasse diagramm to console in text form
        """
        self.face_enumeration()
        self.enumeration_interface.Output()

    def draw_hasse_diagram(self):
        """
        Process atom-coatom structure and build graph
        """
        self.face_enumeration()
        list_enterpretation = self.enumeration_interface.GraphPostProcessing()
        # list_enterpretation = list_enterpretation[:len(list_enterpretation)-1]
        # list_enterpretation = sorted(list_enterpretation, key=lambda x: x[0])
        graph_interpret = Digraph('Hasse_diagram', filename='hello.gv')
        for pair in list_enterpretation:
            h_node_string = ' '.join(str(x) for x in pair[0])
            g_node_string = ' '.join(str(x) for x in pair[1])
            graph_interpret.edge(h_node_string, g_node_string)


        graph_interpret.view()


if __name__ == "__main__":
    test_face_enum = FaceEnum(Path.cwd() / 'Hasse_diagram' / 'examples' / 'pyr3.txt')
    test_face_enum.face_enumeration()
    test_face_enum.print_text_hasse_diagramm()
    test_face_enum.draw_hasse_diagram()